#include "terrains.h"

Null::Null() {
    typeId = 0;
    img = nullptr;
}

Mud::Mud() {
    typeId = 1;
    img->load(":/img/terrain/mud.png");
}

Grass::Grass() {
    typeId = 2;
    img->load(":/img/terrain/grass.png");
}

Ice::Ice() {
    typeId = 3;
    img->load(":/img/terrain/ice.png");
}
