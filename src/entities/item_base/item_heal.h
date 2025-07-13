#ifndef ITEM_HEAL_H
#define ITEM_HEAL_H

#include "../item.h"

class ItemHeal : public Item
{
public:
    explicit ItemHeal();

    void update() override;
};

#endif // ITEM_HEAL_H
