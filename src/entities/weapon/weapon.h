#ifndef WEAPON_H
#define WEAPON_H

#include "../item_base/item_attack.h"

class Fist : public ItemAttack
{
public:
    explicit Fist();
    ~Fist();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

};

#endif // WEAPON_H
