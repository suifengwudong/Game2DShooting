#include "bullet.h"
#include "../player.h"

Bullet::Bullet(QPointF startPos, QPointF velocity, int m_harm, Player* shooter)
    : ItemAttack("bullet"), m_shooter(shooter)
{
    harm = m_harm;
    setPos(startPos);
    setVel(velocity);
    collideBox = QRectF(0, 0, 16, 8);
    terminalVelocityX = 150.f;
    terminalVelocityY = 150.f;
}

Bullet::~Bullet() {}

void Bullet::update() {
    qDebug() << "Bullet position:" << pos();
    GameObject::update();
}

void Bullet::use(Player* player) {
    player->health -= harm;
    player->onHealthChanged();
}
