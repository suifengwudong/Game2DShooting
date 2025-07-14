#include "bullet.h"
#include "../../physics/physics_engine.h"
#include "../../screen/game_screen.h"

Bullet::Bullet(QPointF startPos, QPointF velocity, int m_harm, Weapon* parent)
{
    harm = m_harm;
    setPos(startPos);
    setVel(velocity);
    collideBox = QRectF(0, 0, 5, 2); // 子弹的碰撞框大小
}

Bullet::~Bullet() {}

void Bullet::update()
{
    setPos(pos() + m_vel);

    // 检查子弹是否超出游戏边界，如果是则删除子弹
    if (pos().x() < 0 || pos().x() > GAME_WIDTH || pos().y() < 0 || pos().y() > GAME_HEIGHT) {
        delete this;
        return;
    }
}
