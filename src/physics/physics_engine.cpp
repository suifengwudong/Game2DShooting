#include "physics_engine.h"
#include "../entities/game_object.h"
#include "../entities/player.h"
#include <cmath>

PhysicsEngine* PhysicsEngine::instance = nullptr;

PhysicsEngine::PhysicsEngine(QObject* parent) : QObject(parent) {}

PhysicsEngine* PhysicsEngine::getInstance() {
    if (instance == nullptr) {
        instance = new PhysicsEngine();
    }
    return instance;
}

void PhysicsEngine::applyGravity(GameObject* object, qreal gravity) {
    QPointF vel = object->vel();
    vel.setY(vel.y() + gravity);
    if (vel.y() > terminalVelocity) {
        vel.setY(terminalVelocity);
    }
    object->setVel(vel);
}

void PhysicsEngine::applyFriction(GameObject* object, qreal friction) {
    QPointF vel = object->vel();
    if (friction != 0) {
        if (vel.x() >= 0) {
            if (vel.x() - friction >= 0) {
                vel.setX(vel.x() - friction);
            } else {
                vel.setX(0);
            }
        } else {
            if (vel.x() + friction <= 0) {
                vel.setX(vel.x() + friction);
            } else {
                vel.setX(0);
            }
        }
    }
    object->setVel(vel);
}

bool PhysicsEngine::checkCollision(GameObject* obj1, GameObject* obj2) {
    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());
    return rect1.intersects(rect2);
}

void PhysicsEngine::handleCollisionResolution(GameObject* obj1, GameObject* obj2, qreal elasticity) {
    if (!checkCollision(obj1, obj2)) return;

    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());

    QPointF obj1Pos = obj1->pos();
    QPointF obj1Vel = obj1->vel();
    QPointF obj2Pos = obj2->pos();
    QPointF obj2Vel = obj2->vel();

    QRectF overlap = rect1.intersected(rect2);

    // mass
    qreal mass1 = 1.0;
    qreal mass2 = 1.0;

    if (obj1->isMovable() && !obj2->isMovable()) {
        if (overlap.width() > overlap.height()) {
            // 垂直
            if (rect1.bottom() <= rect2.top() + overlap.height()) {
                obj1Pos.setY(rect2.top() - rect1.height());
                obj1Vel.setY(0);
                if (auto player = dynamic_cast<Player*>(obj1)) {
                    player->setOnGround(true);
                }
            } else {
                obj1Pos.setY(rect2.bottom());
                obj1Vel.setY(-obj1Vel.y());
            }
        } else {
            // 水平
            if (rect1.right() >= rect2.left() && rect1.left() <= rect2.right()) {
                if (obj1Vel.x() > 0) {
                    obj1Pos.setX(rect2.left() - rect1.width());
                    obj1Vel.setX(0);
                } else if (obj1Vel.x() < 0) {
                    obj1Pos.setX(rect2.right());
                    obj1Vel.setX(0);
                }
            }
        }
    }
    else if (!obj1->isMovable() && obj2->isMovable()) {
        if (overlap.width() > overlap.height()) {
            // 垂直
            if (rect2.bottom() <= rect1.top() + overlap.height()) {
                obj2Pos.setY(rect1.top() - rect2.height());
                obj2Vel.setY(0);
                if (auto player = dynamic_cast<Player*>(obj2)) {
                    player->setOnGround(true);
                }
            } else {
                obj2Pos.setY(rect1.bottom());
                obj2Vel.setY(-obj2Vel.y());
            }
        } else {
            // 水平
            if (rect2.right() >= rect1.left() && rect2.left() <= rect1.right()) {
                if (obj2Vel.x() > 0) {
                    obj2Pos.setX(rect1.left() - rect2.width());
                    obj2Vel.setX(0);
                } else if (obj2Vel.x() < 0) {
                    obj2Pos.setX(rect1.right());
                    obj2Vel.setX(0);
                }
            }
        }
    } else if (obj1->isMovable() && obj2->isMovable()) {
        QPointF relativeVel = obj1Vel - obj2Vel;
        QPointF normal;

        if (overlap.width() > overlap.height()) {
            normal = QPointF(0, 1);
        } else {
            normal = QPointF(1, 0);
        }

        // impulse
        qreal impulse = (-(1 + elasticity) * QPointF::dotProduct(relativeVel, normal)) /
                        (QPointF::dotProduct(normal, normal) * (1/mass1 + 1/mass2));

        obj1Vel += impulse * normal / mass1;
        obj2Vel -= impulse * normal / mass2;

        if (overlap.width() > overlap.height()) {
            if (rect1.bottom() <= rect2.top() + overlap.height()) {
                obj1Pos.setY(rect2.top() - rect1.height());
                if (auto player = dynamic_cast<Player*>(obj1)) {
                    player->setOnGround(true);
                }
            } else if (rect2.bottom() <= rect1.top() + overlap.height()) {
                obj2Pos.setY(rect1.top() - rect2.height());
                if (auto player = dynamic_cast<Player*>(obj2)) {
                    player->setOnGround(true);
                }
            }
        } else {
            if (relativeVel.x() > 0) {
                obj1Pos.setX(rect2.left() - rect1.width());
                obj2Pos.setX(rect1.right());
            } else if (relativeVel.x() < 0) {
                obj1Pos.setX(rect2.right());
                obj2Pos.setX(rect1.left() - rect2.width());
            }
        }
    }

    // margins
    if (obj1->isMovable()) {
        if (obj1Pos.x() < 0) {
            obj1Pos.setX(0);
            obj1Vel.setX(0);
        } else if (obj1Pos.x() + rect1.width() > GAME_WIDTH) {
            obj1Pos.setX(GAME_WIDTH - rect1.width());
            obj1Vel.setX(0);
        }
        if (obj1Pos.y() + rect1.height() > GAME_HEIGHT) {
            obj1Pos.setY(GAME_HEIGHT - rect1.height());
            obj1Vel.setY(0);
        }
        obj1->setPos(obj1Pos);
        obj1->setVel(obj1Vel);
    }

    if (obj2->isMovable()) {
        if (obj2Pos.x() < 0) {
            obj2Pos.setX(0);
            obj2Vel.setX(0);
        } else if (obj2Pos.x() + rect2.width() > GAME_WIDTH) {
            obj2Pos.setX(GAME_WIDTH - rect2.width());
            obj2Vel.setX(0);
        }
        if (obj2Pos.y() + rect2.height() > GAME_HEIGHT) {
            obj2Pos.setY(GAME_HEIGHT - rect2.height());
            obj2Vel.setY(0);
        }
        obj2->setPos(obj2Pos);
        obj2->setVel(obj2Vel);
    }
}

qreal PhysicsEngine::distance(GameObject* obj1, GameObject* obj2) {
    QPointF p1 = obj1->boundingRect().center();
    QPointF p2 = obj2->boundingRect().center();

    qreal dx = p1.x() - p2.x();
    qreal dy = p1.y() - p2.y();

    return std::sqrt(dx * dx + dy * dy);
}
