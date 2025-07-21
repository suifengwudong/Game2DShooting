#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include "screen.h"

namespace Ui {
class SettingsScreen;
}

class SettingsScreen : public Screen
{
    Q_OBJECT

public:
    explicit SettingsScreen(QWidget *parent = nullptr);
    ~SettingsScreen();

signals:
    void backToStartScreen();

private:
    Ui::SettingsScreen *ui;
};

#endif // SETTINGS_SCREEN_H
