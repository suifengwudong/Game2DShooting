#ifndef ITEM_ATTACK_H
#define ITEM_ATTACK_H

#include "../item.h"

class ItemAttack : public Item
{
public:
    explicit ItemAttack();

    void update() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override final;

    int getHarm() const;
    int getAttackCD() const;

protected:
    int harm;
    int attackCD;
};

#endif // ITEM_ATTACK_H
