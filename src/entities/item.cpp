#include "item.h"
#include <QPainter>

Item::Item() {
    img = new QImage();
}

Item::~Item() {
    delete img;
}

void Item::update() {
    ;
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (img && !img->isNull()) {
        painter->drawImage(boundingRect(), *img);
    }
}

QImage* Item::getImage() {
    return img;
}

float Item::getSpawnPR() const {
    return spawnPR;
}

bool Item::isOnGround(Map* gameMap) {
    QPointF gridPos = positionToGrid(pos());
    int gridX = static_cast<int>(gridPos.x());
    int gridY = static_cast<int>(gridPos.y());

    if (gridY + 1 >= gameMap->map.size()) {
        return false;
    }

    // 获取下一行的 Terrain
    Terrain* terrainBelow = gameMap->map[gridY + 1][gridX];

    if (terrainBelow && terrainBelow->getTypeId() != 0) {
        return PhysicsEngine::getInstance()->checkCollision(this, terrainBelow);
    }

    return false;
}
