#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <QObject>

class GameObject;

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
    void handleCollisionResolution(GameObject* obj1, GameObject* obj2, qreal elasticity = 1.0f);
    qreal distance(GameObject* obj1, GameObject* obj2);

private:
    static constexpr qreal m_gravity = 0.5f;
    static constexpr qreal m_friction = 0.5f;
    const qreal terminalVelocity = 15.0f;
};

#endif // PHYSICS_ENGINE_H
