#include "item_heal.h"

ItemHeal::ItemHeal(bool imagable, const QString& itemName) :
    Item(imagable, itemName) {}

void ItemHeal::update() {
    Item::update();
}

int ItemHeal::getHealHealth() const {
    return healHealth;
}
