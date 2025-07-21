#ifndef HUD_H
#define HUD_H

#include "../entities/player.h"
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class HUD;
}

class HUD : public QFrame
{
    Q_OBJECT

public:
    explicit HUD(Player* player, QWidget *parent = nullptr);
    ~HUD();
    void updateHealth(int health);
    void setPlayerImage(QImage* img);
    void setWeaponImage(QImage* img);
    void setDefenseImage(QImage* img);
    void startAttactCDCountingDown();

private:
    Ui::HUD *ui;
    QString m_playerName;
    QGraphicsOpacityEffect *attackCDEffect;
    QPropertyAnimation *opacityAnimation;
};

#endif // HUD_H
