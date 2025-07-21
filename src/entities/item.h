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
<<<<<<< HEAD
    qreal getSpawnPR() const;
    QString getName() const;
=======
    void loadImage();

    qreal getSpawnPR() const;
>>>>>>> 8001155888255e3d75e247fdefe8e84431b38991

    // void startFalling();

protected:
    QImage *img;
    QString name;
    qreal spawnPR;
<<<<<<< HEAD

    // falling item timer
    QTimer* fallTimer;
    QTimer* existTimer;
=======
>>>>>>> 8001155888255e3d75e247fdefe8e84431b38991
};

#endif // ITEM_H
