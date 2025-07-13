#ifndef ITEM_H
#define ITEM_H

#include "game_object.h"

class Item : public GameObject
{
public:
    explicit Item();
    ~Item();

    void update() = 0;
    // boundingRect and collideBox has been defined

    QImage* getImage();

protected:
    QImage *img;
};

#endif // ITEM_H
