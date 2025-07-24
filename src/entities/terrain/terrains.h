#ifndef TERRAINS_H
#define TERRAINS_H

#include "terrain.h"

class Null : public Terrain
{
public:
    explicit Null();
};

class Mud : public Terrain
{
public:
    explicit Mud();
};

class Grass : public Terrain
{
public:
    explicit Grass();
};

class Ice : public Terrain
{
public:
    explicit Ice();
};

#endif // TERRAINS_H
