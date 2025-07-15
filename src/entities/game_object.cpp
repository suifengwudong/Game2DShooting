#include "game_object.h"
#include "config.h"
#include "../physics/physics_engine.h"

GameObject::GameObject(bool movable) :
    movable(movable), m_reactionRange(0) // TODO: real reactive range
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

    PhysicsEngine::getInstance()->applyGravity(this);
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

void GameObject::setReactionRange(qreal range) {
    m_reactionRange = range;
}

QRectF GameObject::reactionRangeRect() const {
    return QRectF(pos().x() - m_reactionRange, pos().y() - m_reactionRange,
                  boundingRect().width() + 2 * m_reactionRange,
                  boundingRect().height() + 2 * m_reactionRange);
}

bool GameObject::isMovable() const {
    return movable;
}

QPointF GameObject::positionToGrid(const QPointF &pos) const {
    int x = static_cast<int>(pos.x()) / TERRAIN_WIDTH;
    int y = static_cast<int>(pos.y()) / TERRAIN_HEIGHT;
    return QPointF(x, y);
}
