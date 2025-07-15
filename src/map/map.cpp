#include "map.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QPainter>

Map::Map(int grid_w, int grid_h, const QString& path)
{
    width = GAME_WIDTH / grid_w;
    height = GAME_HEIGHT / grid_h;
    currentPath = path;

    map = QVector<QVector<Terrain*>>(height, QVector<Terrain*>(width, nullptr));

    // load default map
    load(currentPath);
}

void Map::load(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open map file:" << path;
        return;
    }

    QTextStream in(&file);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int typeId;
            in >> typeId;
            qDebug() << "Loaded terrain at (" << x << ", " << y << ") with typeId: " << typeId;
            switch (typeId) {
            case 0:
                map[y][x] = new Null();
                break;
            case 1:
                map[y][x] = new Mud();
                break;
            case 2:
                map[y][x] = new Grass();
                break;
            case 3:
                map[y][x] = new Ice();
                break;
            default:
                map[y][x] = new Null();
                break;
            }
            if (map[y][x]) {
                map[y][x]->setPos(x * TERRAIN_WIDTH, y * TERRAIN_HEIGHT);
            }
        }
    }

    file.close();
}

void Map::save(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open map file for writing:" << path;
        return;
    }

    QTextStream out(&file);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Terrain* terrain = map[y][x];
            if (terrain) {
                out << terrain->getTypeId() << " ";
            } else {
                out << "0 "; // 默认使用 Null 地形
            }
        }
        out << "\n";
    }

    file.close();
}

void Map::paint(QPainter *painter)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto terrain = map[y][x];
            if (terrain) {
                if (terrain->getTypeId() != 0) {  // 0 代表 Null 地形
                    QImage* img = terrain->getImage();
                    if (img) {
                        painter->drawImage(x * TERRAIN_WIDTH, y * TERRAIN_HEIGHT, *img);
                    }
                }
            }
        }
    }
}
