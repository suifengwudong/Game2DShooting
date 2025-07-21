#include "item_attack.h"

ItemAttack::ItemAttack(bool imagable, const QString& itemName) :
    Item(imagable, itemName) {}

void ItemAttack::update() {
    Item::update();
}

int ItemAttack::getHarm() const {
    return harm;
}

int ItemAttack::getAttackCD() const {
    return attackCD;
}
