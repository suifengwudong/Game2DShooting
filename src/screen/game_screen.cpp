#include "game_screen.h"
#include "../entities/heal/heal.h"
#include "../entities/defense/defense.h"
#include "hud.h"
#include "config.h"
#include <QVBoxLayout>
#include <QRandomGenerator>

GameScreen::GameScreen(QWidget *parent) : Screen(parent)
{
    // 此处不能随意添加Timer有关的代码，防止开始界面卡死
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(QSize(GAME_WIDTH, GAME_HEIGHT));
    gameScene = new QGraphicsScene();
    gameView = new QGraphicsView(gameScene);
    gameView->setParent(this);
    gameView->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(gameView);
    layout->setContentsMargins(0, 0, 0, 0);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &GameScreen::updateGame);
    updateTimer->start(16); // 大约60 FPS
}

GameScreen::~GameScreen() {
    if (updateTimer) {
        updateTimer->stop();
    }

    for (Player* player : players) {
        delete player;
    }
    delete gameScene;
    delete gameView;
    delete gameMap;

    delete updateTimer;
}

void GameScreen::initGame() {
    setStyleSheet("background-color: #CCFFFF;");
    gameScene->setSceneRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
    gameView->setSceneRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
    setWindowTitle(GAME_NAME);

    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gameView->show();

    // paint map
    gameMap = new Map();
    gameMap->load();
    // QPixmap pixmap(GAME_WIDTH, GAME_HEIGHT);
    // QPainter painter(&pixmap);
    // gameMap->paint(&painter);
    // painter.end();
    // gameScene->addPixmap(pixmap);

    Player *player1 = new Player("Player1", 10, 10, this);
    players.append(player1);
    gameScene->addItem(player1);
    player1->setPos((GAME_WIDTH-player1->boundingRect().width())/2, GAME_HEIGHT-player1->boundingRect().height());
    player1->setVel(QPointF(0, -20));

    Player *player2 = new Player("Player2", GAME_WIDTH-380-10, 10, this);
    players.append(player2);
    gameScene->addItem(player2);
    player2->setPos((GAME_WIDTH-player2->boundingRect().width())/2, GAME_HEIGHT-player2->boundingRect().height());
    player2->setVel(QPointF(0, -20));

    for (auto player : players) {
        player->hud->show();
        connect(player, &Player::bulletShot, [this](Bullet* bullet){
            bullets.append(bullet);
            gameScene->addItem(bullet);
        });
    }
}

void GameScreen::keyPressEvent(QKeyEvent *event) {
    for (Player* player : players) {
        player->keyPressEvent(event);
    }
    QWidget::keyPressEvent(event);
}

void GameScreen::keyReleaseEvent(QKeyEvent *event) {
    for (Player* player : players) {
        player->keyReleaseEvent(event);
    }
    QWidget::keyReleaseEvent(event);
}

void GameScreen::checkCollisionWithTerrain(Player *player) {
    for (int y = 0; y < gameMap->map.size(); ++y) {
        for (int x = 0; x < (gameMap->map)[y].size(); ++x) {
            Terrain* terrain = (gameMap->map)[y][x];
            if (terrain && terrain->getTypeId() != 0 /* 忽略Null地形 */) {
                QRectF playerRect = player->boundingRect().translated(player->pos());
                QRectF terrainRect = terrain->boundingRect().translated(terrain->pos());

                auto physicsEngine = PhysicsEngine::getInstance();
                if (physicsEngine->checkCollision(player, terrain)) {
                    physicsEngine->resolveCollision(player, terrain);
                    // 检查碰撞是否来自上方（玩家站在地形上）
                    if (playerRect.bottom() >= terrainRect.top() &&
                        playerRect.bottom() <= terrainRect.top() + 5 /* 阈值 */) {
                        player->setOnGround(true);
                    }
                }

                // check bullets
                for (auto bullet : bullets) {
                    if (physicsEngine->checkCollision(bullet, terrain)) {
                        gameScene->removeItem(bullet);
                        bullets.removeOne(bullet);
                        delete bullet;
                    }
                }
            }
        }
    }
}

void GameScreen::randomSpawnItems() {
    // 随机生成物品的间隔时长，3s - 4s
    int interval = QRandomGenerator::global()->bounded(3000, 4001);
    QTimer::singleShot(interval, this, [this]() {
        // 随机选择物品类型
        float randomValue = QRandomGenerator::global()->generateDouble();
        float cumulativeProbability = 0;
        Item* item = nullptr;
        if (randomValue < (cumulativeProbability += Knife().getSpawnPR())) {
            item = new Knife();
        } else if (randomValue < (cumulativeProbability += SolidBall().getSpawnPR())) {
            item = new SolidBall();
        } else if (randomValue < (cumulativeProbability += Rifle(10).getSpawnPR())) {
            item = new Rifle(10);
        } else if (randomValue < (cumulativeProbability += SniperRifle(5).getSpawnPR())) {
            item = new SniperRifle(5);
        }

        cumulativeProbability = 0;
        if (randomValue < (cumulativeProbability += Bandage().getSpawnPR())) {
            item = new Bandage();
        } else if (randomValue < (cumulativeProbability += MedKit().getSpawnPR())) {
            item = new MedKit();
        } else if (randomValue < (cumulativeProbability += Adrenaline().getSpawnPR())) {
            item = new Adrenaline();
        }

        // cumulativeProbability = 0;
        // if (randomValue < (cumulativeProbability += ItemDefend().getSpawnPR())) {
        //     item = new ItemDefend();
        // }

        if (item) {
            int x = QRandomGenerator::global()->bounded(GAME_WIDTH - item->boundingRect().width());
            int y = 0;
            item->setPos(x, y);

            PhysicsEngine::getInstance()->applyGravity(item);
            gameScene->addItem(item);

            QTimer* fallTimer = new QTimer(this);
            connect(fallTimer, &QTimer::timeout, [this, item, fallTimer]() {
                item->update();

                if (item->isOnGround(gameMap)) {
                    fallTimer->stop();
                    // 掉落到地板上后，一段时间后消失（5s）
                    QTimer::singleShot(5000, [this, item]() {
                        gameScene->removeItem(item);
                        delete item;
                    });
                }
            });
            fallTimer->start(16); // 大约60 FPS
        }

        // 递归调用，继续随机生成物品
        randomSpawnItems();
    });
}

void GameScreen::updateGame() {
    for (Player* player : players) {
        player->update();
        player->setOnGround(false); // 默认设为未站在地面
    }

    // 处理攻击逻辑
    for (Player* attacker : players) {
        if (attacker->isAttacked()) {
            // 获取攻击者的响应范围
            QRectF reactionRange = attacker->reactionRangeRect();
            for (Player* defender : players) {
                if (defender != attacker && reactionRange.intersects(defender->boundingRect().translated(defender->pos()))) {
                    attacker->attack(defender);
                }
            }
        }
    }

    // 处理地形与玩家的碰撞
    for (Player* player : players) {
        checkCollisionWithTerrain(player);
        // qDebug() << player->vel();
    }
}

