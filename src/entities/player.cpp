#include "player.h"
#include "config.h"
#include "../screen/hud.h"
#include "weapon/weapon.h"
#include "defense/defense.h"
#include <QPainter>

Player::Player(QString const playerName, int ax, int ay, QWidget *parent) :
    GameObject(true), name(playerName), health(INIT_HEALTH), facingRight(true)
{
    collideBox = QRectF(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    keys = InputManager::allocateKeyMap(this)->getKeyMap();
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    onGround = false;
    proImg = new QImage();
    if (!proImg->load("://entities/player_default.png")){
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
    if (onGround) {
        // jump and crouch can not happen simutaneously
        if (keyStates[keys->at(0)]) jump();
        else if (keyStates[keys->at(3)]) crouch();
    } else {
        if (keyStates[keys->at(3)]) drop();
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
    setPos(x()-10, y());
    facingRight = false;
}

void Player::right() {
    setPos(x()+10, y());
    facingRight = true;
}

void Player::jump() {
    QPointF m_vel = vel();
    m_vel.setY(-10);
    setVel(m_vel);
}

void Player::crouch(){
    ;
}

void Player::drop() {
    PhysicsEngine::getInstance()->applyGravity(this);
}

void Player::attack(Player* otherPlayer) {
    if (otherPlayer && !onAttackCD) {
        if (auto knife = dynamic_cast<Knife*>(weapon)) {
            // 小刀攻击逻辑
            auto vec = QPoint(0, 0);
            if (facingRight) {
                vec = QPoint(5, 0);
            } else {
                vec = QPoint(-5, 0);
            }

            if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon))
            {
                otherPlayer->health -= weapon->getHarm();
                otherPlayer->setVel(vel() + vec);
                otherPlayer->onHealthChanged();
            }

            onAttackCD = true;
            attackCDTimer.start(ATTACK_CD_TIME);  // Attack CD is 1s

            emit hudStartAttackCDCountingDown();
        } else if (auto solidBall = dynamic_cast<SolidBall*>(weapon)) {
            // 实心球攻击逻辑
            if (solidBall->use()) {
                // 实现斜抛运动逻辑
                // ...

                if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon))
                {
                    otherPlayer->health -= weapon->getHarm();
                    otherPlayer->setVel(vel() + QPoint(5, -5)); // 简单示例
                    otherPlayer->onHealthChanged();
                }

                if (solidBall->use() == false) {
                    delete weapon;
                    weapon = new Fist();
                    hud->setWeaponImage(weapon->getImage());
                }

                onAttackCD = true;
                attackCDTimer.start(ATTACK_CD_TIME);  // Attack CD is 1s

                emit hudStartAttackCDCountingDown();
            }
        } else if (auto rifle = dynamic_cast<Rifle*>(weapon)) {
            // 步枪攻击逻辑
            if (rifle->use()) {
                // 实现直线运动逻辑
                // ...

                if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon))
                {
                    otherPlayer->health -= weapon->getHarm();
                    otherPlayer->setVel(vel() + QPoint(10, 0)); // 简单示例
                    otherPlayer->onHealthChanged();
                }

                if (rifle->use() == false) {
                    delete weapon;
                    weapon = new Fist();
                    hud->setWeaponImage(weapon->getImage());
                }

                onAttackCD = true;
                attackCDTimer.start(ATTACK_CD_TIME);  // Attack CD is 1s

                emit hudStartAttackCDCountingDown();
            }
        } else if (auto sniperRifle = dynamic_cast<SniperRifle*>(weapon)) {
            // 狙击枪攻击逻辑
            if (sniperRifle->use()) {
                // 实现直线运动逻辑
                // ...

                if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon))
                {
                    otherPlayer->health -= weapon->getHarm();
                    otherPlayer->setVel(vel() + QPoint(20, 0)); // 简单示例
                    otherPlayer->onHealthChanged();
                }

                if (sniperRifle->use() == false) {
                    delete weapon;
                    weapon = new Fist();
                    hud->setWeaponImage(weapon->getImage());
                }

                onAttackCD = true;
                attackCDTimer.start(ATTACK_CD_TIME);  // Attack CD is 1s

                emit hudStartAttackCDCountingDown();
            }
        } else {
            // 原有武器攻击逻辑
            auto vec = QPoint(0, 0);
            if (facingRight) {
                vec = QPoint(5, 0);
            } else {
                vec = QPoint(-5, 0);
            }

            if (PhysicsEngine::getInstance()->checkCollision(otherPlayer, weapon))
            {
                otherPlayer->health -= weapon->getHarm();
                otherPlayer->setVel(vel() + vec);
                otherPlayer->onHealthChanged();
            }

            onAttackCD = true;
            attackCDTimer.start(ATTACK_CD_TIME);  // Attack CD is 1s

            emit hudStartAttackCDCountingDown();
        }
    }
}

bool Player::isOnGround() const {
    return onGround;
}

void Player::setOnGround(bool value) {
    onGround = value;
}

bool Player::isAttacked() const {
    return keyStates[keys->at(4)];
}

void Player::onHealthChanged() {
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
