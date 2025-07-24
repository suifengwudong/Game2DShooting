#include "../entities/defense/armor.h"
#include "hud.h"
#include "ui_hud.h"
#include "config.h"

HUD::HUD(Player* player, QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::HUD)
    , m_playerName(player->name)
{
    ui->setupUi(this);
    updateHealth(100);

    attackCDEffect = new QGraphicsOpacityEffect(this);
    ui->graphicsViewWeapon->setGraphicsEffect(attackCDEffect);
    attackCDEffect->setOpacity(1);

    opacityAnimation = new QPropertyAnimation(attackCDEffect, "opacity", this);
    opacityAnimation->setDuration(player->weapon->getAttackCD()); // 动画时长与攻击冷却时间一致
    opacityAnimation->setEasingCurve(QEasingCurve::Linear);
}

HUD::~HUD()
{
    delete ui;
}

void HUD::updateHealth(int health) {
    ui->healthLabel->setText(QString("%1 Health:%2").arg(m_playerName).arg(health));
    ui->progressBar->setValue(health);
}

void HUD::setPlayerImage(QImage* img) {
    if (img) {
        QPixmap pixmap = QPixmap::fromImage(*img);
        pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio);
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->addPixmap(pixmap);
        ui->graphicsViewPlayer->setScene(scene);
        ui->graphicsViewPlayer->setSceneRect(0, 0, 50, 50);
        ui->graphicsViewPlayer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsViewPlayer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsViewPlayer->setFixedSize(50, 50);
    }
}

void HUD::setWeaponImage(QImage* img) {
    if (img) {
        QPixmap pixmap = QPixmap::fromImage(*img);
        pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio);
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->setBackgroundBrush(Qt::white);
        scene->addPixmap(pixmap);
        ui->graphicsViewWeapon->setScene(scene);
        ui->graphicsViewWeapon->setSceneRect(0, 0, 50, 50);
        ui->graphicsViewWeapon->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsViewWeapon->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsViewWeapon->setFixedSize(50, 50);
    }
}


void HUD::setDefenseImage(QImage* img) {
    if (img) {
        QPixmap pixmap = QPixmap::fromImage(*img);
        pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio);
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->setBackgroundBrush(Qt::white);
        scene->addPixmap(pixmap);
        ui->graphicsViewDefense->setScene(scene);
        ui->graphicsViewDefense->setSceneRect(0, 0, 50, 50);
        ui->graphicsViewDefense->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsViewDefense->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsViewDefense->setFixedSize(50, 50);
    }
}

void HUD::updateArmorDurability(int durability, int maxDurability, bool visible) {
    if (!ui->defenseDurabilityOverlay) return;
    if (visible && maxDurability > 0) {
        int barWidth = 50 * durability / maxDurability;
        if (barWidth < 0) barWidth = 0;
        ui->defenseDurabilityOverlay->setGeometry(0, 0, barWidth, 50);
        ui->defenseDurabilityOverlay->show();
    } else {
        ui->defenseDurabilityOverlay->hide();
    }
}

void HUD::startAttactCDCountingDown() {
    attackCDEffect->setOpacity(0.5);

    opacityAnimation->setStartValue(0.5);
    opacityAnimation->setEndValue(1.0);

    opacityAnimation->start();
}
