#include "bullet.h"
#include "../player.h"

Bullet::Bullet(QPointF startPos, QPointF velocity, int m_harm, Player* shooter)
    : ItemAttack(true, "bullet"), m_shooter(shooter)
{
    harm = m_harm;
    setPos(startPos);
    setVel(velocity);
    collideBox = QRectF(0, 0, 50, 25);
    // qDebug() << "Name:" << name;
}

Bullet::~Bullet() {}

void Bullet::update() {
    GameObject::update();
}

void Bullet::use(Player* player) {
    player->health -= harm;
    player->onHealthChanged();
}
