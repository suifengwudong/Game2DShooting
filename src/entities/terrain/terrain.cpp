#include "terrain.h"
#include "config.h"
#include "entities/item.h"
#include "entities/player.h"

Terrain::Terrain() : GameObject(false) {
    img = new QImage();
    collideBox = QRectF(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT);
}

Terrain::~Terrain() {
    delete img;
}

void Terrain::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QImage* Terrain::getImage() {
    return img;
}

int Terrain::getTypeId() {
    return typeId;
}
