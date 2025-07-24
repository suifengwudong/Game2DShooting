#include "heal.h"

Bandage::Bandage() : ItemHeal(true, "bandage") {
    healHealth = 10;
    spawnPR = 0.2;
    collideBox = QRectF(0, 0, 36, 36);
}

Bandage::~Bandage() {}

void Bandage::use(Player* player) {
    player->health += healHealth;
    player->onHealthChanged();
}

MedKit::MedKit() : ItemHeal(true, "medkit") {
    healHealth = 100;
    spawnPR = 0.1;
    collideBox = QRectF(0, 0, 44, 44);
}

MedKit::~MedKit() {}

void MedKit::use(Player* player) {
    player->health += healHealth;
    player->onHealthChanged();
}

Adrenaline::Adrenaline() : ItemHeal(true, "adrenaline") {
    healHealth = 10;
    spawnPR = 0.1;
    collideBox = QRectF(0, 0, 32, 32); 
    effectDuration = 5000;
    tickInterval = 1000;
    usage = 5;
    adrenalineTimer = new QTimer(this);
}

Adrenaline::~Adrenaline() {}

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
        if (healCount >= usage) {
            adrenalineTimer->stop();
            disconnect(adrenalineTimer, &QTimer::timeout, nullptr, nullptr);
        }
    });
}
