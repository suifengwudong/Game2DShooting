#include "game_object.h"
#include "config.h"
#include "../physics/physics_engine.h"
#include "player.h"

GameObject::GameObject(bool movable) :
    movable(movable)
{}

GameObject::~GameObject() {}

QPointF GameObject::vel() const {
    return m_vel;
}

void GameObject::setVel(const QPointF newvel) {
    m_vel = newvel;
}

void GameObject::update() {
    if (!movable) return;

    auto physicsEngine = PhysicsEngine::getInstance();
    auto player = dynamic_cast<Player*>(this);

    if (player && player->isOnGround()) {
        physicsEngine->applyFriction(this);
    } else {
        physicsEngine->applyGravity(this);
        physicsEngine->applyFriction(this);
    }

    setPos(pos() + m_vel);

    // margin check
    if (pos().x() < 0) {
        setPos(0, pos().y());
        m_vel.setX(0);
    }
    else if (pos().x() + boundingRect().width() > GAME_WIDTH) {
        setPos(GAME_WIDTH - boundingRect().width(), pos().y());
        m_vel.setX(0);
    }

    if (pos().y() + boundingRect().height() > GAME_HEIGHT) {
        setPos(pos().x(), GAME_HEIGHT - boundingRect().height());
        m_vel.setY(0);
    }
}

QRectF GameObject::boundingRect() const {
    return collideBox;
}

bool GameObject::isMovable() const {
    return movable;
}

QPointF GameObject::positionToGrid(const QPointF &pos) const {
    int x = static_cast<int>(pos.x()) / TERRAIN_WIDTH;
    int y = static_cast<int>(pos.y()) / TERRAIN_HEIGHT;
    return QPointF(x, y);
}
