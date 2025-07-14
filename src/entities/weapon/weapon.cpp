#include "weapon.h"
#include "bullet.h"
#include <QDebug>
#include <QTimer>

/* FIST */
Fist::Fist() {
    harm = 5;
    attackCD = 1000;
    spawnPR = 0;
    collideBox = QRectF(0, 0, 10, 10);
    if (!img->load("://entities/fist.png")) {
        qDebug() << "Failed to load fist image";
    }
}

Fist::~Fist() {}

bool Fist::use() {
    return true;
}

/* KNIFE */
Knife::Knife() {
    harm = 8;
    attackCD = 1000;
    spawnPR = 0.2;
    collideBox = QRectF(0, 0, 15, 5);
    if (!img->load("://entities/knife.png")) {
        qDebug() << "Failed to load knife image";
    }
}

Knife::~Knife() {}

bool Knife::use() {
    return true;
}

/* SOLIDBALL */
SolidBall::SolidBall() {
    harm = 10;
    attackCD = 1000;
    spawnPR = 0.1;
    collideBox = QRectF(0, 0, 8, 8);
    if (!img->load("://entities/solid_ball.png")) {
        qDebug() << "Failed to load solid ball image";
    }
}

SolidBall::~SolidBall() {}

bool SolidBall::use() {
    return false;
}

/* RIFLE */
Rifle::Rifle(int bulletCount) : bulletCount(bulletCount) {
    harm = 15;
    attackCD = 1000;
    spawnPR = 0.15;
    collideBox = QRectF(0, 0, 5, 2);
    if (!img->load("://entities/rifle.png")) {
        qDebug() << "Failed to load rifle image";
    }
    shootTimer.setInterval(100); // 射击间隔
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
    return new Bullet(startPos, velocity, harm, this);
}

/* SNIPER RIFLE */
SniperRifle::SniperRifle(int bulletCount) : bulletCount(bulletCount) {
    harm = 50;
    attackCD = 1000;
    spawnPR = 0.05;
    collideBox = QRectF(0, 0, 8, 3);
    if (!img->load("://entities/sniper_rifle.png")) {
        qDebug() << "Failed to load sniper rifle image";
    }
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
    return new Bullet(startPos, velocity, harm, this);
}
