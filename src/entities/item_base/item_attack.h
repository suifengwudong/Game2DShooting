#ifndef ITEM_ATTACK_H
#define ITEM_ATTACK_H

#include "../item.h"

class ItemAttack : public Item
{
public:
    explicit ItemAttack();

    void update() override;
    int getHarm();

protected:
    int harm;
};

#endif // ITEM_ATTACK_H
