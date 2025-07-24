#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include "../entities/player.h"
#include "../entities/weapon/bullet.h"
#include "../map/map.h"
#include "screen.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QList>

class GameScreen : public Screen
{
Q_OBJECT
public:
    explicit GameScreen(QWidget *parent = nullptr);
    ~GameScreen();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void initGame();
    void updateGame();
    void pauseGame();
    void resumeGame();

signals:
    void gamePaused(QStringList statuslist);
    void gameEnd(QString &playerName);

private:
    QGraphicsScene *gameScene;
    QGraphicsView *gameView;
    QTimer *updateTimer;
    QTimer *spawnTimer;

    QList<Player*> players;
    QList<Bullet*> bullets;
    QList<SolidBall*> solidBalls;
    QList<Item*> items;
    Map *gameMap;

    PhysicsEngine* phEn;

    void randomSpawnItems();
    void clearGameState();
    // void spawnPlayer(QString playerName, int ax, int ay);
};

#endif // GAME_WIDGET_H
