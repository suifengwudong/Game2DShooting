#include "item.h"

Item::Item() {
    img = new QImage();
}

Item::~Item() {
    delete img;
}

QImage* Item::getImage() {
    return img;
}

float Item::getSpawnPR() const {
    return spawnPR;
}
