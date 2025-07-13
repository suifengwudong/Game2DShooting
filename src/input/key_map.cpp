#include "key_map.h"

KeyMap::KeyMap(int index) {
    switch (index) {
    case 0:
        keyMap = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_E};
        break;
    case 1:
        keyMap = {Qt::Key_I, Qt::Key_J, Qt::Key_K, Qt::Key_L, Qt::Key_U};
        break;
    case 2:
        keyMap = {Qt::Key_Up, Qt::Key_Left, Qt::Key_Down, Qt::Key_Right, Qt::Key_0};
        break;
    default:
        keyMap = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D, Qt::Key_E};
        break;
    }
}

const QList<Qt::Key> *KeyMap::getKeyMap() const {
    return &keyMap;
}
