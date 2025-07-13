#include "physics_engine.h"
#include "../entities/game_object.h"
#include "../entities/Player.h"
#include <cmath>

PhysicsEngine* PhysicsEngine::instance = nullptr;

PhysicsEngine::PhysicsEngine(QObject* parent) : QObject(parent) {}

PhysicsEngine* PhysicsEngine::getInstance() {
    if (instance == nullptr) {
        instance = new PhysicsEngine();
    }
    return instance;
}

void PhysicsEngine::applyGravity(GameObject* object) {
    QPointF vel = object->vel();
    vel.setY(vel.y() + gravity);
    if (vel.y() > terminalVelocity) {
        vel.setY(terminalVelocity);
    }
    object->setVel(vel);
}

bool PhysicsEngine::checkCollision(GameObject* obj1, GameObject* obj2) {
    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());
    return rect1.intersects(rect2);
}

void PhysicsEngine::resolveCollision(GameObject* obj1, GameObject* obj2, float elasticity) {
    QRectF rect1 = obj1->boundingRect().translated(obj1->pos());
    QRectF rect2 = obj2->boundingRect().translated(obj2->pos());

    QPointF obj1Pos = obj1->pos();
    QPointF obj1Vel = obj1->vel();
    QPointF obj2Pos = obj2->pos();
    QPointF obj2Vel = obj2->vel();

    QRectF overlap = rect1.intersected(rect2);

    if (obj1->isMovable() && !obj2->isMovable()) {
        if (overlap.width() > overlap.height()) {
            // 处理垂直方向碰撞
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
            // 检查是否真的是水平碰撞
            if (rect1.right() >= rect2.left() && rect1.left() <= rect2.right()) {
                if (obj1Vel.x() > 0) { // 玩家向右移动
                    obj1Pos.setX(rect2.left() - rect1.width());
                    obj1Vel.setX(-obj1Vel.x());
                } else if (obj1Vel.x() < 0) { // 玩家向左移动
                    obj1Pos.setX(rect2.right());
                    obj1Vel.setX(-obj1Vel.x());
                }
            }
        }
    }
    else if (!obj1->isMovable() && obj2->isMovable()) {
        if (overlap.width() > overlap.height()) {
            // 处理垂直方向碰撞
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
            // 检查是否真的是水平碰撞
            if (rect2.right() >= rect1.left() && rect2.left() <= rect1.right()) {
                if (obj2Vel.x() > 0) { // 玩家向右移动
                    obj2Pos.setX(rect1.left() - rect2.width());
                    obj2Vel.setX(-obj2Vel.x());
                } else if (obj2Vel.x() < 0) { // 玩家向左移动
                    obj2Pos.setX(rect1.right());
                    obj2Vel.setX(-obj2Vel.x());
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

        float impulse = (-(1 + elasticity) * QPointF::dotProduct(relativeVel, normal)) /
                        (QPointF::dotProduct(normal, normal) * (1/1.0 + 1/1.0));

        obj1Vel += impulse * normal / 1.0;
        obj2Vel -= impulse * normal / 1.0;

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
            if (relativeVel.x() > 0) { // obj1 相对向右移动
                obj1Pos.setX(rect2.left() - rect1.width());
                obj2Pos.setX(rect1.right());
            } else if (relativeVel.x() < 0) { // obj1 相对向左移动
                obj1Pos.setX(rect2.right());
                obj2Pos.setX(rect1.left() - rect2.width());
            }
        }
    }

    obj1->setPos(obj1Pos);
    obj1->setVel(obj1Vel);
    obj2->setPos(obj2Pos);
    obj2->setVel(obj2Vel);
}
