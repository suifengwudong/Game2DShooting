#include "item.h"
#include "config.h"
#include "../map/map.h"
#include <QPainter>
#include <QDebug>

Item::Item(bool imagable, const QString& itemName) :
    GameObject(true), name(itemName) {
    img = new QImage();
    if (imagable) loadImage();
    spawnPR = 0.5; // Default spawn probability
}

Item::~Item() {
    delete img;
}

void Item::update() {
    // not to update in base class

    if (!onGround()) {
        phEn->applyGravity(this);
        setPos(pos() + m_vel);
    }

    QRect gridRect = getGrid();

    int minX = gridRect.left();
    int minY = gridRect.top();
    int maxX = gridRect.right();
    int maxY = gridRect.bottom();

    // Check if the item is out of bounds
    checkBoundaries();

    auto gameMap = Map::getInstance();
    for (int y = maxY; y >= minY; --y) {
        for (int x = minX; x <= maxX; ++x) {
            Terrain* terrain = gameMap->map[y][x];
            if (terrain && terrain->getTypeId() != 0) {
                QRectF overlap = terrain->getPosRectF().intersected(getPosRectF());
                if (!overlap.isEmpty()) {
                    setPos(pos().x(), terrain->pos().y() - boundingRect().height());
                    setVel(QPointF(0, 0)); // Stop falling
                    // Handle collision with terrain
                    terrain->onCollidedWith(this);
                    checkBoundaries();
                    return; // Stop further processing if collided
                }
            }
        }
    }
}

void Item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    if (img && !img->isNull()) {
        painter->drawImage(boundingRect(), *img, QRectF(0, 0, img->width(), img->height()));
    }
    qDebug() << boundingRect();
}

QImage* Item::getImage() {
    return img;
}

void Item::loadImage() {
    QString path = ":/img/entities/" + name + ".png";
    if (!img->load(path)) {
        qDebug() << name << " failed to load";
    } else {
        qDebug() << name << " succeeded to load";
    }
}

qreal Item::getSpawnPR() const {
    return spawnPR;
}

QString Item::getName() const {
    return name;
}

