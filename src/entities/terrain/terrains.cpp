#include "terrains.h"

Null::Null() {
    typeId = 0;
    img = nullptr;
}

void Null::onCollidedWith(Player *player) {
    // 空地不处理碰撞
    Q_UNUSED(player);
}

Mud::Mud() {
    typeId = 1;
    img->load(":/img/terrain/mud.png");
}

void Mud::onCollidedWith(Player *player) {
    // 在泥地上，玩家速度减半
    // player->setVel(player->vel() * 0.5);
}

Grass::Grass() {
    typeId = 2;
    img->load(":/img/terrain/grass.png");
}

void Grass::onCollidedWith(Player *player) {
    // 在草地上，玩家速度正常
    // player->setVel(player->vel() * 1.0);
}

Ice::Ice() {
    typeId = 3;
    img->load(":/img/terrain/ice.png");
}

void Ice::onCollidedWith(Player *player) {
    // 在冰面上，玩家速度加倍
    // player->setVel(player->vel() * 2.0);
}