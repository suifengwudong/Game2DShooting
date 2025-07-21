#ifndef HEAL_H
#define HEAL_H

#include "../item_base/item_heal.h"
#include <QTimer>

class Bandage : public ItemHeal
{
public:
    explicit Bandage();
    ~Bandage();

    void use(Player* player) override;
};

class MedKit : public ItemHeal
{
public:
    explicit MedKit();
    ~MedKit();

    void use(Player* player) override;
};

class Adrenaline : public ItemHeal
{
Q_OBJECT
public:
    explicit Adrenaline();
    ~Adrenaline();

    void use(Player* player) override;

signals:
    void sucHealing();

private:
    int effectDuration;
    int tickInterval;
    int usage;
    QTimer* adrenalineTimer;
};

#endif // HEAL_H
