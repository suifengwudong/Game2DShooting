#ifndef START_SCREEN_H
#define START_SCREEN_H

#include "screen.h"
#include <QWidget>

namespace Ui {
class StartScreen;
}

class StartScreen : public Screen
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

signals:
    void startGameClicked();
    void settingsClicked();
    void mapEditorClicked();

private:
    Ui::StartScreen *ui;
};

#endif // START_SCREEN_H
