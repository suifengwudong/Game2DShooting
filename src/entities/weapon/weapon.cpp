#include "weapon.h"
#include "bullet.h"

Weapon::Weapon(const QString& weaponName) :
    ItemAttack(true, weaponName) 
{}

/* FIST */
Fist::Fist() :
    Weapon("fist")
{
    harm = 8;
    attackCD = 400;
    spawnPR = 0;
    collideBox = QRectF(0, 0, 40, 40);
}

Fist::~Fist() {}

bool Fist::use() {
    return true;
}

/* KNIFE */
Knife::Knife() :
    Weapon("knife") {
    harm = 18;
    attackCD = 600;
    spawnPR = 0.2;
    collideBox = QRectF(0, 0, 40, 40);
}

Knife::~Knife() {}

bool Knife::use() {
    return true;
}

/* SOLIDBALL */
SolidBall::SolidBall(Player* shooter, float terminalVx, float terminalVy) :
    Weapon("solidball"), MotionProfile(terminalVx, terminalVy), m_shooter(shooter)
{
    harm = 35;
    name = "solidball";
    attackCD = 800;
    spawnPR = 0.1;
    collideBox = QRectF(0, 0, 32, 32);
    terminalVelocityX = 12.0f;
    terminalVelocityY = 16.0f;
}

SolidBall::~SolidBall() {}

bool SolidBall::use() {
    return false;
}

void SolidBall::update() {
    GameObject::update();
}

/* RIFLE */
Rifle::Rifle(int bulletCount) :
    Weapon("rifle"), bulletCount(bulletCount)
{
    harm = 25;
    attackCD = 500;
    spawnPR = 0.15;
    collideBox = QRectF(0, 0, 80, 24);
}

Rifle::~Rifle() {}

bool Rifle::use() {
    if (bulletCount > 0) {
        bulletCount--;
        return true;
    }
    return false;
}

Bullet* Rifle::createBullet(QPointF startPos, QPointF direction, Player* shooter) {
    QPointF velocity = direction * 10;
    return new Bullet(startPos, velocity, harm, shooter);
}

/* SNIPER RIFLE */
SniperRifle::SniperRifle(int bulletCount) :
    Weapon("sniper_rifle"), bulletCount(bulletCount)
{
    harm = 80;
    attackCD = 1200;
    spawnPR = 0.05;
    collideBox = QRectF(0, 0, 100, 28);
}

SniperRifle::~SniperRifle() {}

bool SniperRifle::use() {
    if (bulletCount > 0) {
        bulletCount--;
        return true;
    }
    return false;
}

Bullet* SniperRifle::createBullet(QPointF startPos, QPointF direction, Player* shooter) {
    QPointF velocity = direction * 10;
    return new Bullet(startPos, velocity, harm, shooter);
}
