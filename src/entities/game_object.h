#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../physics/physics_engine.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPointF>

class GameObject : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit GameObject(bool movable = true);
    virtual ~GameObject();

    // life cycle management
    virtual void update();
    virtual QRectF boundingRect() const override final;

    // velocity
    QPointF vel() const;
    void setVel(const QPointF newvel);

    // isMovable
    bool isMovable() const;

    // point to grid
    QPointF positionToGrid(const QPointF &pos) const;

protected:
    bool movable;
    QRectF collideBox;

    QPointF m_vel;
};

#endif // GAME_OBJECT_H
