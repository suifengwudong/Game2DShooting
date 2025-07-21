#include "bullet.h"
#include "../player.h"

Bullet::Bullet(QPointF startPos, QPointF velocity, int m_harm) :
    ItemAttack(false, "bullet")
{
    harm = m_harm;
    setPos(startPos);
    setVel(velocity);
    collideBox = QRectF(0, 0, 5, 2); // 子弹的碰撞框大小
}

Bullet::~Bullet() {}

void Bullet::update()
{
    GameObject::update();
}

void Bullet::use(Player* player) {
    player->health -= harm;
    player->onHealthChanged();
}
