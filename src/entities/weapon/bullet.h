#ifndef BULLET_H
#define BULLET_H

#include "weapon.h"
#include <QTimer>

class Player;

class Bullet : public ItemAttack
{
    Q_OBJECT
public:
    explicit Bullet(QPointF startPos, QPointF velocity, int harm);
    ~Bullet();

    void update() override;
    void use(Player* player);
};

#endif // BULLET_H
