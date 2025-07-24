#ifndef ARMOR_H
#define ARMOR_H

#include "../item_base/item_defend.h"
#include <QString>

// 护甲基类
class Armor : public ItemDefend {
public:
    explicit Armor(const QString& name) : ItemDefend(true, name) {}
    virtual ~Armor() {}
    // 返回是否对该类型武器防御
    virtual bool defend(const QString& weaponType) const = 0;
    // 处理伤害，返回最终玩家应受伤害
    virtual int absorbDamage(const QString& weaponType, int damage) = 0;
    // 是否有耐久
    virtual bool hasDurability() const { return false; }
    virtual int getDurability() const { return -1; }
};

// 锁子甲/轻甲

class ChainArmor : public Armor {
public:
    ChainArmor();
    bool defend(const QString& weaponType) const override;
    int absorbDamage(const QString& weaponType, int damage) override;
};

class BulletproofVest : public Armor {
public:
    BulletproofVest();
    bool defend(const QString& weaponType) const override;
    int absorbDamage(const QString& weaponType, int damage) override;
    bool hasDurability() const override;
    int getDurability() const override;
private:
    int durability;
};

#endif // ARMOR_H
