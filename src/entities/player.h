#ifndef PLAYER_H
#define PLAYER_H

#include "../input/input_manager.h"
#include "defense/defense.h"
#include "weapon/weapon.h"
#include "weapon/bullet.h"
#include "../physics/motion_profile.h"
#include <QMap>
#include <QTimer>

class HUD;
class Bullet; // 前向声明

class Player : public GameObject, public MotionProfile
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
    void pick(Item* item);
    // void shoot();

    bool isPicking() const;
    bool isAttacking() const;

    // status
    QString name;
    int health;
    Weapon* weapon;
    ItemDefend* defense;
    QImage* img;
    QList<QImage*> imgList;

    HUD* hud;

    QImage* getImage();
    void setStealth(bool v);
    void onCollidedWithTerrain(int terrainType);

signals:
    void gameEnd(QString &playerName);
    void healthChanged();
    void bulletShot(Bullet *bullet);
    void solidBallCast(SolidBall *solidBall);
    void hudStartAttackCDCountingDown();

public slots:
    void onHealthChanged();
    // void onAdrenalineUsed();

private:
    QImage *proImg;
    bool onAttackCD;
    bool facingRight;
    const QList<Qt::Key> *keys;
    QMap<Qt::Key, bool> keyStates;
    QTimer attackCDTimer;
    QTimer* adrenalineTimer = nullptr;

    // 地形特效相关
    bool isStealth = false;
    int currentTerrainType;

    void updateWeaponPosition();
    void checkTerrainEffect();

    friend class InputManager;
};

#endif // PLAYER_H
