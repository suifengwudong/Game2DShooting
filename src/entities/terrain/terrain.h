#ifndef TERRAIN_H
#define TERRAIN_H

#include "../game_object.h"
#include <QImage>

class Terrain : public GameObject
{
    Q_OBJECT
public:
    explicit Terrain();
    ~Terrain();

    QImage* getImage();
    int getTypeId();

    static void loadImage(const QString& path);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override final;

protected:
    static QImage* img;
    int typeId;
};

#endif // TERRAIN_H
