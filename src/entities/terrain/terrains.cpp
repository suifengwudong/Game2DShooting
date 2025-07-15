#include "terrains.h"

Null::Null() {
    typeId = 0;
    img = nullptr;
}

Mud::Mud() {
    typeId = 1;
    img->load("://terrain/mud.png");
}

Grass::Grass() {
    typeId = 2;
    img->load("://terrain/grass.png");
}

Ice::Ice() {
    typeId = 3;
    img->load("://terrain/ice.png");
}
