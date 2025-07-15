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
        QRectF itemRect = boundingRect().translated(pos());
        QRectF terrainRect = terrainBelow->boundingRect().translated(terrainBelow->pos());

        auto physicsEngine = PhysicsEngine::getInstance();
        return physicsEngine->checkCollision(this, terrainBelow);
    }

    return false;
}
