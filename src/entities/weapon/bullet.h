#ifndef BULLET_H
#define BULLET_H

#include "weapon.h"
#include "../../physics/motion_profile.h"
#include <QTimer>

class Player;

class Bullet : public ItemAttack, public MotionProfile
{
public:
    Bullet(QPointF startPos, QPointF velocity, int m_harm, Player* shooter = nullptr);
    ~Bullet();
    void setShooter(Player* shooter) { m_shooter = shooter; }
    Player* getShooter() const { return m_shooter; }
    void update() override;
    void use(Player* player);
private:
    Player* m_shooter = nullptr;
};

#endif // BULLET_H
