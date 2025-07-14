#include "item_attack.h"

ItemAttack::ItemAttack() {}

void ItemAttack::update() {}

void ItemAttack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //
}

int ItemAttack::getHarm() const {
    return harm;
}

int ItemAttack::getAttackCD() const {
    return attackCD;
}
