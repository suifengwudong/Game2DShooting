#include "armor.h"

// 目前所有实现都在头文件内，若后续有复杂逻辑可在此扩展。

ChainArmor::ChainArmor() : Armor("chain_armor") {
    spawnPR = 0.5;
    collideBox = QRectF(0, 0, 50, 50);
}

bool ChainArmor::defend(const QString& weaponType) const {
    return weaponType == "fist" || weaponType == "knife";
}

int ChainArmor::absorbDamage(const QString& weaponType, int damage) {
    if (weaponType == "fist") return 0;
    if (weaponType == "knife") return damage / 2;
    return damage;
}

BulletproofVest::BulletproofVest() : Armor("bulletproof_vest"), durability(60) {
    spawnPR = 0.3;
    collideBox = QRectF(0, 0, 50, 50);
}

bool BulletproofVest::defend(const QString& weaponType) const {
    return weaponType == "bullet";
}

int BulletproofVest::absorbDamage(const QString& weaponType, int damage) {
    if (weaponType == "bullet") {
        int absorb = damage * 0.7;
        durability -= absorb;
        if (durability < 0) durability = 0;
        return damage - absorb;
    }
    return damage;
}

bool BulletproofVest::hasDurability() const { return durability > 0; }
int BulletproofVest::getDurability() const { return durability; }
