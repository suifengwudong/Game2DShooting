#ifndef ITEM_H
#define ITEM_H

#include "game_object.h"
#include "../map/map.h"

class Item : public GameObject
{
public:
    explicit Item();
    ~Item();

    void update() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override final;
    // boundingRect and collideBox has been defined

    QImage* getImage();
    float getSpawnPR() const;

    bool isOnGround(Map* map);

protected:
    QImage *img;
    float spawnPR;
};

#endif // ITEM_H
