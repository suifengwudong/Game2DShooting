#ifndef HEAL_H
#define HEAL_H

#include "../item_base/item_heal.h"
#include <QTimer>

class Bandage : public ItemHeal
{
public:
    explicit Bandage();

    void use(Player* player) override;
};

class MedKit : public ItemHeal
{
public:
    explicit MedKit();

    void use(Player* player) override;
};

class Adrenaline : public ItemHeal
{
public:
    explicit Adrenaline();

    void use(Player* player) override;

signals:
    void sucHealing();

private:
    int effectDuration;
    int tickInterval;
    QTimer* adrenalineTimer;
};

#endif // HEAL_H
