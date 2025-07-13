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
    void applyGravity(GameObject* object);
    bool checkCollision(GameObject* obj1, GameObject* obj2);
    void resolveCollision(GameObject* obj1, GameObject* obj2, float elasticity = 1.0f);
    // void inresolveCollision(GameObject* player, GameObject* terrain);

private:
    const float gravity = 0.5f;
    const float terminalVelocity = 15.0f;
    // const float elasticity = 0.8f;
};

#endif // PHYSICS_ENGINE_H
