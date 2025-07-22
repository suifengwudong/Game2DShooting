#ifndef WEAPON_H
#define WEAPON_H

#include "../item_base/item_attack.h"
#include "../../physics/motion_profile.h"
#include <QTimer>

class Bullet;

class Weapon: public ItemAttack
{
public:
    explicit Weapon(const QString& weaponName = "fist");
    ~Weapon() {}
    virtual bool use() = 0;
};

class Fist : public Weapon
{
public:
    explicit Fist();
    ~Fist();
    bool use() override;
};

class Knife : public Weapon
{
public:
    explicit Knife();
    ~Knife();
    bool use() override;
};

class Player;
class SolidBall : public Weapon, public MotionProfile {
public:
    SolidBall(Player* shooter = nullptr, float terminalVx = 12.0f, float terminalVy = 16.0f);
    ~SolidBall();
    bool use() override;
    void update() override;
    void setShooter(Player* shooter) { m_shooter = shooter; }
    Player* getShooter() const { return m_shooter; }
private:
    Player* m_shooter = nullptr;
};

class Rifle : public Weapon
{
public:
    explicit Rifle(int bulletCount);
    ~Rifle();
    bool use() override;
    Bullet* createBullet(QPointF startPos, QPointF direction, Player* shooter = nullptr);
private:
    int bulletCount;
};

class SniperRifle : public Weapon
{
public:
    explicit SniperRifle(int bulletCount);
    ~SniperRifle();
    bool use() override;
    Bullet* createBullet(QPointF startPos, QPointF direction, Player* shooter = nullptr);
private:
    int bulletCount;
};

#endif // WEAPON_H
