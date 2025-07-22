#ifndef ITEM_H
#define ITEM_H

#include "game_object.h"
#include <QTimer>

class Item : public GameObject
{
public:
    explicit Item(bool imagable = true, const QString& itemName = "");
    ~Item();

    void update() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override final;
    // boundingRect and collideBox has been defined

    void loadImage();
    QImage* getImage();
    qreal getSpawnPR() const;
    QString getName() const;

    // void startFalling();

protected:
    QImage *img;
    QString name;
    qreal spawnPR;

    // falling item timer
    QTimer* fallTimer;
    QTimer* existTimer;
};

#endif // ITEM_H
