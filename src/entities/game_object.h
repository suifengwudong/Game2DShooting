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

    // reactive range
    void setReactionRange(qreal range);
    QRectF reactionRangeRect() const;

    // isMovable
    bool isMovable() const;

protected:
    QPointF m_vel;
    bool movable;
    QRectF collideBox;
    qreal m_reactionRange;
};

#endif // GAME_OBJECT_H
