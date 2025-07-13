#include "defense.h"

Shirt::Shirt() {
    img = new QImage();
    if (!img->load("://entities/shirt.png")) {
        // 处理加载失败的情况
        qDebug() << "Failed to load fist image";
    }
}

Shirt::~Shirt() {
    delete img;
}

void Shirt::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}
