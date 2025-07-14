#ifndef PLAYER_H
#define PLAYER_H

#include "../input/input_manager.h"
#include "item_base/item_defend.h"
#include "weapon/weapon.h"
#include "weapon/bullet.h"
#include <QMap>
#include <QTimer>

class HUD;
class Bullet; // 前向声明

class Player : public GameObject
{
    Q_OBJECT
public:
    explicit Player(QString const playerName, int ax, int ay, QWidget *parent = nullptr);
    ~Player();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void virtual update() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // actions
    void left();
    void right();
    void jump();
    void crouch();
    void drop();
    void attack(Player* otherPlayer);
    // void shoot();

    bool isOnGround() const;
    void setOnGround(bool value);
    bool isAttacked() const;

    // status
    QString name;
    int health;
    Weapon* weapon;
    ItemDefend* defense;

    HUD* hud;

    QImage* getImage();

signals:
    void healthChanged();
    void bulletShot(Bullet *bullet);
    void hudStartAttackCDCountingDown();

public slots:
    void onHealthChanged();
    void onAdrenalineUsed();

private:
    QImage *proImg;
    bool onGround;
    bool onAttackCD;
    bool facingRight;
    const QList<Qt::Key> *keys;
    QMap<Qt::Key, bool> keyStates;
    QTimer attackCDTimer;
    QTimer* adrenalineTimer = nullptr;

    void updateWeaponPosition();

    friend class InputManager;
};

#endif // PLAYER_H
