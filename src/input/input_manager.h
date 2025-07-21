#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "key_map.h"
#include <QWidget>
#include <QKeyEvent>

class Player;

class InputManager : public QWidget
{
    Q_OBJECT
public:
    InputManager(QObject *parent = nullptr);

    static KeyMap* allocateKeyMap(Player *player);

private:
    QHash<Player*, KeyMap*> playerKeyMaps;
    static int index;
};

#endif // INPUT_MANAGER_H
