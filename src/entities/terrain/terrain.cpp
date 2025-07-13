#include "terrain.h"
#include "config.h"

QImage* Terrain::img = nullptr;

Terrain::Terrain() : GameObject(false) {
    collideBox = QRectF(0, 0, TERRAIN_WIDTH, TERRAIN_HEIGHT);
}

Terrain::~Terrain() {}

void Terrain::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{}

QImage* Terrain::getImage() {
    return img;
}

int Terrain::getTypeId() {
    return typeId;
}

void Terrain::loadImage(const QString& path) {
    if (!img) {
        img = new QImage();
        if (!img->load(path)) {
            // 处理加载失败的情况
            qDebug() << "Failed to load terrain image: " << path;
        }
    }
}
