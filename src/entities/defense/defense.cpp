#include "defense.h"

Shirt::Shirt() {
    spawnPR = 0.1;
    if (!img->load("://entities/shirt.png")) {
        // 处理加载失败的情况
        qDebug() << "Failed to load fist image";
    }
}

Shirt::~Shirt() {
    delete img;
}
