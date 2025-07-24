#ifndef MAP_H
#define MAP_H

#include "../entities/terrain/terrains.h"
#include "config.h"
#include <QVector>

class Map
{
public:
    static Map* getInstance(int grid_w = SMALL_GRID_SIZE, int grid_h = SMALL_GRID_SIZE);
    QVector<QVector<Terrain*>> map;

    // file flows
    void load();
    void load(const QString& path);
    void save(const QString& path = DEFAULT_MAP_PATH);

    void paint(QPainter *painter);

    // gets
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getTerrainTypeAt(const QPoint &gridPos) const;

private:
    Map(int grid_w, int grid_h, const QString& path = DEFAULT_MAP_PATH);
    ~Map();
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

    static Map* instance;
    QString currentPath;
    int width;
    int height;
};

#endif // MAP_H
