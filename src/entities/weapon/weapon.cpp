#include "weapon.h"
#include <QDebug>
#include <QTimer>

Fist::Fist() {
    harm = 5;
    collideBox = QRectF(0, 0, 10, 10);
    img = new QImage();
    if (!img->load("://entities/fist.png")) {
        qDebug() << "Failed to load fist image";
    }
}

Fist::~Fist() {
    delete img;
}

void Fist::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}

Knife::Knife() {
    harm = 8;
    collideBox = QRectF(0, 0, 15, 5);
    img = new QImage();
    if (!img->load("://entities/knife.png")) {
        qDebug() << "Failed to load knife image";
    }
}

Knife::~Knife() {
    delete img;
}

void Knife::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}

SolidBall::SolidBall(int count) : ballCount(count) {
    harm = 10;
    collideBox = QRectF(0, 0, 8, 8);
    img = new QImage();
    if (!img->load("://entities/solid_ball.png")) {
        qDebug() << "Failed to load solid ball image";
    }
}

SolidBall::~SolidBall() {
    delete img;
}

bool SolidBall::use() {
    if (ballCount > 0) {
        ballCount--;
        return true;
    }
    return false;
}

void SolidBall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}

Rifle::Rifle(int bulletCount) : bulletCount(bulletCount) {
    harm = 15;
    collideBox = QRectF(0, 0, 5, 2);
    img = new QImage();
    if (!img->load("://entities/rifle.png")) {
        qDebug() << "Failed to load rifle image";
    }
    shootTimer.setInterval(100); // 射击间隔
}

Rifle::~Rifle() {
    delete img;
}

bool Rifle::use() {
    if (bulletCount > 0 && !shootTimer.isActive()) {
        bulletCount--;
        shootTimer.start();
        return true;
    }
    return false;
}

void Rifle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}

SniperRifle::SniperRifle(int bulletCount) : bulletCount(bulletCount) {
    harm = 50;
    collideBox = QRectF(0, 0, 8, 3);
    img = new QImage();
    if (!img->load("://entities/sniper_rifle.png")) {
        qDebug() << "Failed to load sniper rifle image";
    }
    shootTimer.setInterval(1000); // 射击间隔
}

SniperRifle::~SniperRifle() {
    delete img;
}

bool SniperRifle::use() {
    if (bulletCount > 0 && !shootTimer.isActive()) {
        bulletCount--;
        shootTimer.start();
        return true;
    }
    return false;
}

void SniperRifle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}
