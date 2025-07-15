#ifndef ITEM_DEFEND_H
#define ITEM_DEFEND_H

#include "../item.h"

class ItemDefend : public Item
{
public:
    explicit ItemDefend();

    void update() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override final;
};

#endif // ITEM_DEFEND_H
