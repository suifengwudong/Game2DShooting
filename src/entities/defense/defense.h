#ifndef DEFENSE_H
#define DEFENSE_H

#include "../item_base/item_defend.h"
#include "armor.h"

class Shirt : public ItemDefend
{
public:
    explicit Shirt();
    ~Shirt();

};

#endif // DEFENSE_H
