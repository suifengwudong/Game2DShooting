#include "weapon.h"
#include "bullet.h"
#include <QDebug>
#include <QTimer>

Weapon::Weapon(const QString& weaponName) :
    ItemAttack(true, weaponName) {}

/* FIST */
Fist::Fist() :
    Weapon("fist")
{
    harm = 5;
    attackCD = 1000;
    spawnPR = 0;
    collideBox = QRectF(0, 0, 10, 10);
}

Fist::~Fist() {}

bool Fist::use() {
    return true;
}

/* KNIFE */
Knife::Knife() :
    Weapon("knife") {
    harm = 8;
    attackCD = 1000;
    spawnPR = 0.2;
    collideBox = QRectF(0, 0, 15, 5);
}

Knife::~Knife() {}

bool Knife::use() {
    return true;
}

/* SOLIDBALL */
SolidBall::SolidBall() :
    Weapon("solidball")
{
    harm = 10;
    name = "solidball";
    attackCD = 1000;
    spawnPR = 0.1;
    collideBox = QRectF(0, 0, 8, 8);
}

SolidBall::~SolidBall() {}

bool SolidBall::use() {
    return false;
}

/* RIFLE */
Rifle::Rifle(int bulletCount) :
    Weapon("rifle"), bulletCount(bulletCount)
{
    harm = 15;
    attackCD = 1000;
    spawnPR = 0.15;
    collideBox = QRectF(0, 0, 5, 2);
    shootTimer.setInterval(100);
}

Rifle::~Rifle() {}

bool Rifle::use() {
    if (bulletCount > 0 && !shootTimer.isActive()) {
        bulletCount--;
        shootTimer.start();
        return true;
    }
    return false;
}

Bullet* Rifle::createBullet(QPointF startPos, QPointF direction) {
    QPointF velocity = direction * 10; // 子弹速度
    return new Bullet(startPos, velocity, harm);
}

/* SNIPER RIFLE */
SniperRifle::SniperRifle(int bulletCount) :
    Weapon("sniper_rifle"), bulletCount(bulletCount)
{
    harm = 50;
    attackCD = 1000;
    spawnPR = 0.05;
    collideBox = QRectF(0, 0, 8, 3);
    shootTimer.setInterval(1000); // 射击间隔
}

SniperRifle::~SniperRifle() {}

bool SniperRifle::use() {
    if (bulletCount > 0 && !shootTimer.isActive()) {
        bulletCount--;
        shootTimer.start();
        return true;
    }
    return false;
}

Bullet* SniperRifle::createBullet(QPointF startPos, QPointF direction) {
    QPointF velocity = direction * 10; // 子弹速度
    return new Bullet(startPos, velocity, harm);
}
