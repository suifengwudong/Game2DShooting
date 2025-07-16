#ifndef ITEM_ATTACK_H
#define ITEM_ATTACK_H

#include "../item.h"

class ItemAttack : public Item
{
public:
    explicit ItemAttack(bool imagable = true, const QString& itemName = "");;

    void update() override;

    int getHarm() const;
    int getAttackCD() const;

protected:
    int harm;
    int attackCD;
};

#endif // ITEM_ATTACK_H
