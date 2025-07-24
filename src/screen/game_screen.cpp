#include "game_screen.h"
#include "../entities/heal/heal.h"
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

    phEn = PhysicsEngine::getInstance();
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

    delete updateTimer;
    delete spawnTimer;
}


void GameScreen::clearGameState() {
    // 1. 清理旧玩家
    for (Player* player : players) {
        gameScene->removeItem(player);
        delete player;
    }
    players.clear();

    // 2. 清理旧子弹
    for (Bullet* bullet : bullets) {
        gameScene->removeItem(bullet);
        delete bullet;
    }
    bullets.clear();

    // 3. 清理旧实心球
    for (SolidBall* solidBall : solidBalls) {
        gameScene->removeItem(solidBall);
        delete solidBall;
    }
    solidBalls.clear();

    // 4. 清理旧道具
    for (Item* item : items) {
        gameScene->removeItem(item);
        delete item;
    }
    items.clear();

    // 5. 清理场景所有内容
    gameScene->clear();
}

void GameScreen::initGame() {
    clearGameState();

    setStyleSheet("background-color: #CCFFFF;");
    gameScene->setSceneRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
    gameView->setSceneRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
    setWindowTitle(GAME_NAME);

    gameView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gameView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gameView->show();

    // paint map, map should be initialised before gameObjects
    gameMap = Map::getInstance();
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
        connect(player, &Player::solidBallCast, [this](SolidBall* solidBall){
            solidBalls.append(solidBall);
            gameScene->addItem(solidBall);
        });
        connect(player, &Player::gameEnd, [this](QString &playerName){
            emit gameEnd(playerName);
        });
    }

    spawnTimer->setInterval(15000); // 每 15 秒随机生成物品
    connect(spawnTimer, &QTimer::timeout, this, &GameScreen::randomSpawnItems);
    spawnTimer->start();
}

void GameScreen::keyPressEvent(QKeyEvent *event) {
    for (Player* player : players) {
        player->keyPressEvent(event);
    }
    if (event->key() == Qt::Key_P) {
        pauseGame();
        return;
    }
    Screen::keyPressEvent(event);
}

void GameScreen::pauseGame() {
    if (updateTimer) updateTimer->stop();
    // 收集玩家状态
    QStringList statusList;
    for (Player* player : players) {
        QString weaponName = player->weapon ? player->weapon->getName() : "无武器";
        statusList << QString("%1  血量:%2  武器:%3").arg(player->name).arg(player->health).arg(weaponName);
    }
    // 传递给PauseScreen
    emit gamePaused(statusList);
}

void GameScreen::resumeGame() {
    if (updateTimer) updateTimer->start(16);
}

void GameScreen::keyReleaseEvent(QKeyEvent *event) {
    for (Player* player : players) {
        player->keyReleaseEvent(event);
    }
    QWidget::keyReleaseEvent(event);
}

void GameScreen::randomSpawnItems() {
    for (Item* item : items) {
        gameScene->removeItem(item);
        delete item;
    }
    items.clear();
    int spawnCount = QRandomGenerator::global()->bounded(1, 3); // 每次生成1~2个
    for (int i = 0; i < spawnCount; ++i) {
        std::vector<Item*> candidates = {
            new Knife(), new SolidBall(), new Rifle(10), new SniperRifle(5),
            new Bandage(), new MedKit(), new Adrenaline(), new ChainArmor(), new BulletproofVest()
        };
        double r = QRandomGenerator::global()->generateDouble();
        double acc = 0;
        Item* item = nullptr;
        for (auto* candidate : candidates) {
            acc += candidate->getSpawnPR();
            if (r < acc) {
                item = candidate;
                break;
            }
            delete candidate;
        }
        if (!item) item = new Knife(); // 保底
        qDebug() << "Spawned item:" << item->getName();
        int x = QRandomGenerator::global()->bounded(0, static_cast<int>(GAME_WIDTH - item->boundingRect().width()));
        int y = QRandomGenerator::global()->bounded(0, TERRAIN_HEIGHT);
        item->setPos(x, y);
        item->setOnGround(false);
        items.append(item);
        gameScene->addItem(item);
    }
}

void GameScreen::updateGame() {
    // update elements in gameScene

    for (Player* player : players) {
        // update player and status
        player->update();
        // player->setStealth(false);
        // player->setOnGround(false);
    }

    for (Item* item : items) {
        // update item
        item->update();
    }

    for (Bullet* bullet : bullets) {
        // update bullet
        bullet->update();
    }

    for (SolidBall* solidBall : solidBalls) {
        // update solid ball
        solidBall->update();
    }    

    // process collisions and actions
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
        if (player->isPicking()) {
            for (Item* item: items) {
                if (phEn->distance(player, item) < 120) {
                    player->pick(item);
                    gameScene->removeItem(item);
                    items.removeOne(item);
                    delete item;
                    break;
                }
            }
        }
    }

    for (auto bullet : bullets) {
        // update bullet
        bullet->update();

        // check player and bullet collision（排除发射者）
        for (auto player : players) {
            if (player == bullet->getShooter()) continue;
            if (PhysicsEngine::getInstance()->checkCollision(player, bullet)) {
                bullet->use(player);
                // qDebug() << "Bullet hit player:" << player->name << "Health:" << player->health;
                gameScene->removeItem(bullet);
                bullets.removeOne(bullet);
                delete bullet;
            }
        }
    }

    for (auto bullet : bullets) {
        if (bullet->onGround() || bullet->isOutside()) {
            gameScene->removeItem(bullet);
            bullets.removeOne(bullet);
            delete bullet;
            continue; // 如果子弹落地，跳过后续处理
        }

        // check player and bullet collision
        for (auto player : players) {
            if (player == bullet->getShooter()) continue;
            if (phEn->checkCollision(player, bullet)) {
                bullet->use(player);
                gameScene->removeItem(bullet);
                bullets.removeOne(bullet);
                delete bullet;
                break;
            }
        }
    }

    for (auto solidBall : solidBalls) {
        if (solidBall->onGround() || solidBall->isOutside()) {
            gameScene->removeItem(solidBall);
            solidBalls.removeOne(solidBall);
            delete solidBall;
            continue; // 如果实心球落地，跳过后续处理
        }

        // check player and solid ball collision（排除发射者）
        for (auto player : players) {
            if (player == solidBall->getShooter()) continue;
            if (phEn->checkCollision(player, solidBall)) {
                player->health -= solidBall->getHarm();
                player->onHealthChanged();
                gameScene->removeItem(solidBall);
                solidBalls.removeOne(solidBall);
                delete solidBall;
                break;
            }
        }
    }

}

