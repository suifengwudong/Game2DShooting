#include "item_defend.h"

ItemDefend::ItemDefend(bool imagable, const QString& itemName) :
    Item(imagable, itemName) {}

void ItemDefend::update() {
    Item::update();
}
