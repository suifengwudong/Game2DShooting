#ifndef BULLET_H
#define BULLET_H

#include "weapon.h"
#include <QTimer>

class Bullet : public ItemAttack
{
    Q_OBJECT
public:
    explicit Bullet(QPointF startPos, QPointF velocity, int harm, Weapon* parent = nullptr);
    ~Bullet();

    void update() override;
};

#endif // BULLET_H
