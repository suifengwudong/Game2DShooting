#ifndef ITEM_HEAL_H
#define ITEM_HEAL_H

#include "../item.h"
#include "../player.h"

class ItemHeal : public Item
{
public:
    explicit ItemHeal(bool imagable = true, const QString& itemName = "");

    void update() override;

    virtual void use(Player* player) = 0;

    int getHealHealth() const;

protected:
    int healHealth;
};

#endif // ITEM_HEAL_H
