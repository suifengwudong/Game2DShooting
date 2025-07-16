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

    spawnTimer = new QTimer(this);
}

GameScreen::~GameScreen() {
    if (updateTimer) {
        updateTimer->stop();
    }
    if (spawnTimer) {
        spawnTimer->stop();
    }

    for (Player* player : players) {
        delete player;
    }
    delete gameScene;
    delete gameView;
    delete gameMap;

    delete updateTimer;
    delete spawnTimer;
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
    QPixmap pixmap(GAME_WIDTH, GAME_HEIGHT);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    gameMap->paint(&painter);
    painter.end();
    gameScene->addPixmap(pixmap);

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

    int interval = QRandomGenerator::global()->bounded(3000, 4001);
    spawnTimer->singleShot(interval, this, [this]() {
        randomSpawnItems();
        // 继续下一次生成
        int nextInterval = QRandomGenerator::global()->bounded(3000, 4001);
        spawnTimer->singleShot(nextInterval, this, [this]() {
            randomSpawnItems();
        });
    });
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
    QRectF playerRect = player->boundingRect().translated(player->pos());
    // 计算玩家所在的网格范围
    int minX = static_cast<int>(playerRect.left() / TERRAIN_WIDTH) - 1;
    int maxX = static_cast<int>(playerRect.right() / TERRAIN_WIDTH) + 1;
    int minY = static_cast<int>(playerRect.top() / TERRAIN_HEIGHT) - 1;
    int maxY = static_cast<int>(playerRect.bottom() / TERRAIN_HEIGHT) + 1;

    minX = std::max(0, minX);
    maxX = std::min(maxX, static_cast<int>(gameMap->map[0].size()) - 1);
    minY = std::max(0, minY);
    maxY = std::min(maxY, static_cast<int>(gameMap->map.size()) - 1);

    auto physicsEngine = PhysicsEngine::getInstance();

    // 遍历玩家周围的方块
    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Terrain* terrain = gameMap->map[y][x];
            if (terrain && terrain->getTypeId() != 0 /* 忽略Null地形 */) {
                physicsEngine->handleCollisionResolution(player, terrain);

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
    // 随机选择物品类型
    qreal randomValue = QRandomGenerator::global()->generateDouble();
    qreal cumulativeProbability = 0;

    Item* item1 = nullptr;
    Item* item2 = nullptr;
    // Item* item3 = nullptr;

    if (randomValue < (cumulativeProbability += Knife().getSpawnPR())) {
        item1 = new Knife();
    } else if (randomValue < (cumulativeProbability += SolidBall().getSpawnPR())) {
        item1 = new SolidBall();
    } else if (randomValue < (cumulativeProbability += Rifle(10).getSpawnPR())) {
        item1 = new Rifle(10);
    } else if (randomValue < (cumulativeProbability += SniperRifle(5).getSpawnPR())) {
        item1 = new SniperRifle(5);
    }
    if (item1) items.append(item1);

    cumulativeProbability = 0;
    if (randomValue < (cumulativeProbability += Bandage().getSpawnPR())) {
        item2 = new Bandage();
    } else if (randomValue < (cumulativeProbability += MedKit().getSpawnPR())) {
        item2 = new MedKit();
    } else if (randomValue < (cumulativeProbability += Adrenaline().getSpawnPR())) {
        item2 = new Adrenaline();
    }
    if (item2) items.append(item2);

    // cumulativeProbability = 0;
    // if (randomValue < (cumulativeProbability += ItemDefend().getSpawnPR())) {
    //     item = new ItemDefend();
    // }

    for (auto item : items) {
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
}

void GameScreen::updateGame() {
    for (Player* player : players) {
        // update player and status
        player->update();
        player->setOnGround(false);
    }

    for (Player* player : players) {
        // attacks
        if (player->isAttacking()) {
            for (Player* defender : players) {
                if (defender != player) {
                    player->attack(defender);
                }
            }
        }

        // pick items
        // if (player->isPicking()) {
        //     for (Item* item: items) {
        //         if (PhysicsEngine::getInstance()->distance(player, item) < 100) {
        //             player->pick(item);
        //         }
        //     }
        // }
    }

    for (auto bullet : bullets) {
        // update bullet
        bullet->update();

        // check player and bullet collision
        for (auto player : players) {
            if (PhysicsEngine::getInstance()->checkCollision(player, bullet)) {
                bullet->use(player);
            }
        }
    }

    for (Player* player : players) {
        // check collision with terrains
        checkCollisionWithTerrain(player);
    }

}

