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
    if (vel.y() > terminalVelocity_y) {
        vel.setY(terminalVelocity_y);
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
    
    if (std::abs(vel.x()) > terminalVelocity_x) {
        if (vel.x() > 0) {
            vel.setX(terminalVelocity_x);
        } else {
            vel.setX(-terminalVelocity_x);
        }
    }

    object->setVel(vel);
}

bool PhysicsEngine::checkCollision(GameObject* obj1, GameObject* obj2) {
    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());
    return rect1.intersects(rect2);
}

Direction PhysicsEngine::getCollisionDirection(GameObject* obj1, GameObject* obj2) {
    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());
    return getCollisionDirection(rect1, rect2);
}

Direction PhysicsEngine::getCollisionDirection(const QRectF& rect1, const QRectF& rect2) {
    QRectF overlap = rect1.intersected(rect2);
    qreal width = overlap.width();
    qreal height = overlap.height();

    if (width >= height) {
        if (rect1.center().y() < rect2.center().y()) {
            return Direction::TOP; // obj1 is above obj2
        } else {
            return Direction::BOTTOM; // obj1 is below obj2
        }
    } else {
        if (rect1.center().x() < rect2.center().x()) {
            return Direction::LEFT; // obj1 is left of obj2
        } else {
            return Direction::RIGHT; // obj1 is right of obj2
        }
    }
}

CollisionInfo PhysicsEngine::getCollisionInfo(GameObject* obj1, GameObject* obj2) {
    CollisionInfo info;

    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());
   
    auto overlap = rect1.intersected(rect2);
    if (overlap.isEmpty()) {
        throw std::runtime_error("No overlap detected in collision info.");
    }

    info.obj1 = obj1;
    info.obj2 = obj2;
    info.overlap = overlap;
    info.direction = getCollisionDirection(rect1, rect2); 

    return info;
}

void PhysicsEngine::handleCollision(CollisionInfo &info) {
    if (!info.obj1 || !info.obj2) return;

    if (!info.obj1->isMovable() && info.obj2->isMovable()) {
        std::swap(info.obj1, info.obj2);
    }

    if (info.obj1->isMovable() && !info.obj2->isMovable()) {
        switch (info.direction) {
            case Direction::TOP:
                info.obj1->setPos(QPointF(info.obj1->pos().x(), info.obj2->pos().y() - info.obj1->boundingRect().height()));
                info.obj1->setVel(QPointF(info.obj1->vel().x(), 0));
                info.obj1->setOnGround(true);
                break;
            case Direction::BOTTOM:
                info.obj1->setPos(QPointF(info.obj1->pos().x(), info.obj2->pos().y() + info.obj2->boundingRect().height()));
                info.obj1->setVel(QPointF(info.obj1->vel().x(), 0));
                break;
            case Direction::LEFT:
                info.obj1->setPos(QPointF(info.obj2->pos().x() - info.obj1->boundingRect().width(), info.obj1->pos().y()));
                info.obj1->setVel(QPointF(0, info.obj1->vel().y()));
                break;
            case Direction::RIGHT:
                info.obj1->setPos(QPointF(info.obj2->pos().x() + info.obj2->boundingRect().width(), info.obj1->pos().y()));
                info.obj1->setVel(QPointF(0, info.obj1->vel().y()));
                break;
        }
    }
}

qreal PhysicsEngine::distance(GameObject* obj1, GameObject* obj2) {
    QPointF p1 = obj1->boundingRect().center();
    QPointF p2 = obj2->boundingRect().center();

    qreal dx = p1.x() - p2.x();
    qreal dy = p1.y() - p2.y();

    return std::sqrt(dx * dx + dy * dy);
}

/*COLLISION INFO*/
void CollisionInfo::print() const {
    qDebug() << "Collision Info:";
    qDebug() << "Object 1:" << obj1->objectName() << "at" << obj1->pos();
    qDebug() << "Object 2:" << obj2->objectName() << "at" << obj2->pos();
    qDebug() << "Direction:" << direction;
    qDebug() << "Overlap Rect:" << overlap;
}
