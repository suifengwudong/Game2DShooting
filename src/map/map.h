#ifndef MAP_H
#define MAP_H

#include "../entities/terrain/terrains.h"
#include "config.h"
#include <QVector>

class Map
{
public:
    Map(int grid_w = SMALL_GRID_SIZE, int grid_h = SMALL_GRID_SIZE, const QString& path = DEFAULT_MAP_PATH);
    QVector<QVector<Terrain*>> map;

    void load(const QString& path = DEFAULT_MAP_PATH);
    void save(const QString& path = DEFAULT_MAP_PATH);
    void paint(QPainter *painter);

    // gets
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int width;
    int height;
    QString currentPath;
};

#endif // MAP_H
