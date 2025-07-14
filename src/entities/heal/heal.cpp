#include "heal.h"

Bandage::Bandage() {
    healHealth = 10;
}

MedKit::MedKit() {
    healHealth = 100;
}

Adrenaline::Adrenaline() {
    healHealth = 10;
    effectDuration = 5000;
    tickInterval = 1000;
    adrenalineTimer = new QTimer(this);
}

void Adrenaline::use(Player* player) {
    int healCount = 0;
    adrenalineTimer->start(tickInterval);

    connect(adrenalineTimer, &QTimer::timeout, [this, player, &healCount]() {
        player->health += this->healHealth;
        if (player->health > 100) {
            player->health = 100;
        }
        player->onHealthChanged();

        healCount++;
        if (healCount >= 5) {
            adrenalineTimer->stop();
            disconnect(adrenalineTimer, &QTimer::timeout, nullptr, nullptr);
        }
    });
}
