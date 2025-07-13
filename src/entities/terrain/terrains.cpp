#include "terrains.h"

Null::Null() {
    typeId = 0;
    img = nullptr;
}

Mud::Mud() {
    typeId = 1;
    loadImage("://terrain/mud.png");
}

Grass::Grass() {
    typeId = 2;
    loadImage("://terrain/grass.png");
}

Ice::Ice() {
    typeId = 3;
    loadImage("://terrain/ice.png");
}
