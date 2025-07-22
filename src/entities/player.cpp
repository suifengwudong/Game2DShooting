#include "player.h"
#include "config.h"
#include "../screen/hud.h"
#include "weapon/weapon.h"
#include "defense/defense.h"
#include "heal/heal.h"
#include <QPainter>
#include <QDebug>

Player::Player(QString const playerName, int ax, int ay, QWidget *parent) :
    GameObject(true), name(playerName), health(INIT_HEALTH), facingRight(true)
{
    collideBox = QRectF(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    setTerminalVelocityX(10.0f);
    setTerminalVelocityY(15.0f);
    keys = InputManager::allocateKeyMap(this)->getKeyMap();
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    proImg = new QImage();
    if (!proImg->load(":/img/entities/player_default.png")){
        qDebug() << "Failed to load player image";
    }

    weapon = new Fist();
    defense = new Shirt();

    // intialize HUD
    hud = new HUD(this, parent);
    hud->setAttribute(Qt::WA_TranslucentBackground);
    hud->setWindowFlags (Qt::FramelessWindowHint);
    hud->setGeometry(ax, ay, HUD_WIDTH, HUD_HEIGHT);
    hud->setPlayerImage(getImage());
    hud->setWeaponImage(weapon->getImage());
    hud->setDefenseImage(defense->getImage());
    hud->show();

    // initailize Keys
    for (const auto& key : *keys) {
        keyStates[key] = false;
    }

    // attack CD and Animation
    onAttackCD = false;
    attackCDTimer.setSingleShot(false);
    connect(&attackCDTimer, &QTimer::timeout, [this]{
        onAttackCD = false;
    });
    connect(this, &Player::hudStartAttackCDCountingDown,
            this->hud, &HUD::startAttactCDCountingDown);
}

Player::~Player() {
    delete hud;
    delete proImg;
    delete weapon;
    delete defense;
}

void Player::keyPressEvent(QKeyEvent *event) {
    Qt::Key key = static_cast<Qt::Key>(event->key());
    if (keys->contains(key)) {
        keyStates[key] = true;
    }
    // 不在这里执行具体操作，只更新按键状态
    GameObject::keyPressEvent(event);
}

void Player::keyReleaseEvent(QKeyEvent *event) {
    Qt::Key key = static_cast<Qt::Key>(event->key());
    if (keys->contains(key)) {
        keyStates[key] = false;
    }
    // 不在这里执行具体操作，只更新按键状态
    GameObject::keyReleaseEvent(event);
}

void Player::update() {
    if (onGround()) {
        // jump and crouch can not happen simutaneously
        if (keyStates[keys->at(0)]) jump();
        else if (keyStates[keys->at(2)]) crouch();
    } else {
        if (keyStates[keys->at(2)]) drop();
    }

    if (keyStates[keys->at(1)]) {
        left();
    }
    if (keyStates[keys->at(3)]) {
        right();
    }
    GameObject::update();
    updateWeaponPosition();
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->fillRect(collideBox, Qt::red);  // 红色矩形
}

void Player::left() {
    setVel(QPointF(vel().x()-5, vel().y()));
    facingRight = false;
}

void Player::right() {
    setVel(QPointF(vel().x()+5, vel().y()));
    facingRight = true;
}

void Player::jump() {
    setVel(QPointF(vel().x(), vel().y()-15));
    setOnGround(false);
}

void Player::crouch() {
    // keyStates locked
    keyStates[keys->at(0)] = false;
    keyStates[keys->at(1)] = false;
    keyStates[keys->at(3)] = false;
}

void Player::drop() {
    PhysicsEngine::getInstance()->applyGravity(this);
}

void Player::attack(Player* otherPlayer) {
    if (otherPlayer && !onAttackCD) {
        // 优先远程武器
        if (dynamic_cast<SolidBall*>(weapon)) {
            SolidBall* solidBall = new SolidBall(this);
            solidBall->setPos(pos() + QPointF(boundingRect().width()/4, boundingRect().height()/4));
            if (facingRight) {
                solidBall->setVel(vel() + QPointF(15, -10));
            } else {
                solidBall->setVel(vel() + QPointF(-15, -10));
            }
            emit solidBallCast(solidBall);
            onAttackCD = true;
            attackCDTimer.start(weapon->getAttackCD());
            emit hudStartAttackCDCountingDown();
        } else if (dynamic_cast<Rifle*>(weapon)) {
            Bullet* bullet;
            if (facingRight) {
                bullet = new Bullet(pos(), QPointF(35, 0), NORMAL_BULLET, this);
            } else {
                bullet = new Bullet(pos(), QPointF(-35, 0), NORMAL_BULLET, this);
            }
            emit bulletShot(bullet);
            onAttackCD = true;
            attackCDTimer.start(weapon->getAttackCD());
            emit hudStartAttackCDCountingDown();
        } else if (dynamic_cast<SniperRifle*>(weapon)) {
            Bullet* bullet;
            if (facingRight) {
                bullet = new Bullet(pos(), QPointF(5, 0), SNIPER_BULLET, this);
            } else {
                bullet = new Bullet(pos(), QPointF(-5, 0), SNIPER_BULLET, this);
            }
            emit bulletShot(bullet);
            onAttackCD = true;
            attackCDTimer.start(weapon->getAttackCD());
            emit hudStartAttackCDCountingDown();
        } else if (dynamic_cast<Knife*>(weapon)) {
            if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon)) {
                otherPlayer->health -= weapon->getHarm();
                otherPlayer->onHealthChanged();
                onAttackCD = true;
                attackCDTimer.start(weapon->getAttackCD());
                emit hudStartAttackCDCountingDown();
            }
        } else if (dynamic_cast<Fist*>(weapon)) {
            QPoint vec(0, 0);
            if (facingRight) {
                vec = QPoint(5, 0);
            } else {
                vec = QPoint(-5, 0);
            }
            if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon)) {
                otherPlayer->health -= weapon->getHarm();
                otherPlayer->setVel(vel() + vec);
                otherPlayer->onHealthChanged();
                onAttackCD = true;
                attackCDTimer.start(weapon->getAttackCD());
                emit hudStartAttackCDCountingDown();
            }
        }

        if (!weapon->use()) {
            delete weapon;
            weapon = new Fist();
            hud->setWeaponImage(weapon->getImage());
        }
    }
}

void Player::pick(Item* item) {
    // 优化武器拾取逻辑
    auto createWeaponByType = [](Item* item) -> Weapon* {
        if (dynamic_cast<Knife*>(item)) return new Knife();
        if (dynamic_cast<SolidBall*>(item)) return new SolidBall();
        if (dynamic_cast<Rifle*>(item)) return new Rifle(10); // 可根据实际 item 状态传递子弹数
        if (dynamic_cast<SniperRifle*>(item)) return new SniperRifle(5);
        return nullptr;
    };

    Weapon* newWeapon = createWeaponByType(item);
    if (newWeapon) {
        if (weapon) delete weapon;
        weapon = newWeapon;
        hud->setWeaponImage(weapon->getImage());
        return;
    }

    if (dynamic_cast<ItemDefend*>(item)) {
        if (defense) delete defense;
        defense = new Shirt(); // 可根据实际类型扩展
        hud->setDefenseImage(defense->getImage());
    } else if (dynamic_cast<ItemHeal*>(item)) {
        auto healing = static_cast<ItemHeal*>(item);
        healing->use(this);
    }
}

bool Player::isPicking() const {
    return keyStates[keys->at(2)] && onGround();
}

bool Player::isAttacking() const {
    return keyStates[keys->at(4)];
}

void Player::onHealthChanged() {
    if (health > MAX_HEALTH) {
        health = MAX_HEALTH;
    } else if (health < 0) {
        health = 0;
    }
    hud->updateHealth(health);
    if (health <= 0) {
        // TODO: death
    }
}

void Player::updateWeaponPosition() {
    QPointF weaponPos = pos();
    if (facingRight) {
        weaponPos.setX(weaponPos.x() + boundingRect().width());
    } else {
        weaponPos.setX(weaponPos.x() - weapon->boundingRect().width());
    }
    weapon->setPos(weaponPos);
}

QImage* Player::getImage() {
    return proImg;
}
