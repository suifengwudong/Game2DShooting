#ifndef WEAPON_H
#define WEAPON_H

#include "../item_base/item_attack.h"
#include <QTimer>

class Fist : public ItemAttack
{
public:
    explicit Fist();
    ~Fist();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

};

class Knife : public ItemAttack
{
public:
    explicit Knife();
    ~Knife();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class SolidBall : public ItemAttack
{
public:
    explicit SolidBall(int count);
    ~SolidBall();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    bool use();

private:
    int ballCount;
};

class Rifle : public ItemAttack
{
public:
    explicit Rifle(int bulletCount);
    ~Rifle();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    bool use();

private:
    int bulletCount;
    QTimer shootTimer;
};

class SniperRifle : public ItemAttack
{
public:
    explicit SniperRifle(int bulletCount);
    ~SniperRifle();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    bool use();

private:
    int bulletCount;
    QTimer shootTimer;
};

#endif // WEAPON_H
