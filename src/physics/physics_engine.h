#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <QObject>
#include <QRectF>

class GameObject;

enum Direction {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

struct CollisionInfo {
    GameObject* obj1;
    GameObject* obj2;
    Direction direction;
    QRectF overlap;

    void print() const;
};

class PhysicsEngine : public QObject {
    Q_OBJECT
private:
    static PhysicsEngine* instance;
    PhysicsEngine(QObject* parent = nullptr);
    PhysicsEngine(const PhysicsEngine&) = delete;
    PhysicsEngine& operator=(const PhysicsEngine&) = delete;

public:
    static PhysicsEngine* getInstance();
    void applyGravity(GameObject* object, qreal gravity = m_gravity);
    void applyFriction(GameObject* object, qreal friction = m_friction);
    bool checkCollision(GameObject* obj1, GameObject* obj2);
    CollisionInfo getCollisionInfo(GameObject* obj1, GameObject* obj2);
    void handleCollision(CollisionInfo &info);
    qreal distance(GameObject* obj1, GameObject* obj2);

private:
    static constexpr qreal m_gravity = 0.596f;
    static constexpr qreal m_friction = 1.0f;
    const qreal terminalVelocity_x = 10.0f;
    const qreal terminalVelocity_y = 15.0f;

    // get collision direction
    Direction getCollisionDirection(GameObject* obj1, GameObject* obj2);
    Direction getCollisionDirection(const QRectF& rect1, const QRectF& rect2);
};

#endif // PHYSICS_ENGINE_H
