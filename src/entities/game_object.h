#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../physics/physics_engine.h"
#include "../physics/motion_profile.h"
#include <QObject>
#include <QGraphicsItem>
#include <QPointF>

class Map;

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

    // onGround
    bool onGround() const;
    void setOnGround(bool value);
    bool checkNotOnGround();

    // is outside
    bool isOutside() const;
    void checkBoundaries();

    // point to grid
    QPoint positionToGrid(const QPointF &pos) const;
    QRectF getPosRectF() const;
    QRectF getGridRectF() const;
    QRect getGrid() const;
    QRect getGrid(const QPointF pf) const;

protected:
    bool movable;
    bool grounded;
    QRectF collideBox;
    PhysicsEngine* phEn;

    QPointF m_vel;

    void processCollisionWithTerrains(Map* gameMap);
};

#endif // GAME_OBJECT_H
