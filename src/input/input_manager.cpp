#include "input_manager.h"

int InputManager::index = 0;

InputManager::InputManager(QObject *parent) : QWidget(nullptr) {
    setFocusPolicy(Qt::StrongFocus);
}

KeyMap* InputManager::allocateKeyMap(Player *player) {
    KeyMap *keyMap = new KeyMap(index);
    index++;
    return keyMap;
}
