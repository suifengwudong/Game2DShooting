#ifndef WEAPON_H
#define WEAPON_H

#include "../item_base/item_attack.h"
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

class SolidBall : public Weapon
{
public:
    explicit SolidBall();
    ~SolidBall();
    bool use() override;
    // void use(Player* player);
    void update() override;
};

class Rifle : public Weapon
{
public:
    explicit Rifle(int bulletCount);
    ~Rifle();
    bool use() override;
    Bullet* createBullet(QPointF startPos, QPointF direction);
private:
    int bulletCount;
    QTimer shootTimer;
};

class SniperRifle : public Weapon
{
public:
    explicit SniperRifle(int bulletCount);
    ~SniperRifle();
    bool use() override;
    Bullet* createBullet(QPointF startPos, QPointF direction);
private:
    int bulletCount;
    QTimer shootTimer;
};

#endif // WEAPON_H
