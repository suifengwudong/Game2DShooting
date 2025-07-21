#ifndef TERRAINS_H
#define TERRAINS_H

#include "terrain.h"

class Null : public Terrain
{
public:
    explicit Null();
    void onCollidedWith(Player *player) override final;
};

class Mud : public Terrain
{
public:
    explicit Mud();
    void onCollidedWith(Player *player) override final;
};

class Grass : public Terrain
{
public:
    explicit Grass();
    void onCollidedWith(Player *player) override final;
};

class Ice : public Terrain
{
public:
    explicit Ice();
    void onCollidedWith(Player *player) override final;
};

#endif // TERRAINS_H
