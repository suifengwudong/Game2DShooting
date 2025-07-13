#ifndef DEFENSE_H
#define DEFENSE_H

#include "../item_base/item_defend.h"

class Shirt : public ItemDefend
{
public:
    explicit Shirt();
    ~Shirt();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

#endif // DEFENSE_H
