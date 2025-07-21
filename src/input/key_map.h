#ifndef KEY_MAP_H
#define KEY_MAP_H

#include <Qt>
#include <QList>

class KeyMap
{
public:
    KeyMap(int index);

    const QList<Qt::Key> *getKeyMap() const;

private:
    QList<Qt::Key> keyMap;
};
\
#endif // KEY_MAP_H
