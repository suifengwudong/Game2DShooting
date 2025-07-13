#ifndef ITEM_DEFEND_H
#define ITEM_DEFEND_H

#include "../item.h"

class ItemDefend : public Item
{
public:
    explicit ItemDefend();

    void update() override;
};

#endif // ITEM_DEFEND_H
