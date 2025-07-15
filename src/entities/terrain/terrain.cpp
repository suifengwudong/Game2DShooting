#include "terrain.h"
#include "config.h"

Terrain::Terrain() : GameObject(false) {
    img = new QImage();
    collideBox = QRectF(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT);
}

Terrain::~Terrain() {
    delete img;
}

void Terrain::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{}

QImage* Terrain::getImage() {
    return img;
}

int Terrain::getTypeId() {
    return typeId;
}

