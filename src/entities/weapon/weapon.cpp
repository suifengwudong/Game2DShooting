#include "weapon.h"

Fist::Fist() {
    harm = 5;
    collideBox = QRectF(0, 0, 10, 10);
    img = new QImage();
    if (!img->load("://entities/fist.png")) {
        // 处理加载失败的情况
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
