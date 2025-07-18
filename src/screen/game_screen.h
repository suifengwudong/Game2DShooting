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
public:
    explicit GameScreen(QWidget *parent = nullptr);
    ~GameScreen();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void initGame();
    void updateGame();

private:
    QGraphicsScene *gameScene;
    QGraphicsView *gameView;
    QTimer *updateTimer;
    QTimer *spawnTimer;

    QList<Player*> players;
    QList<Bullet*> bullets;
    QList<Item*> items;
    Map *gameMap;

    void checkCollisionWithTerrain(Player *player);
    void randomSpawnItems();
};

#endif // GAME_WIDGET_H
