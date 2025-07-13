#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "screen/game_screen.h"
#include "screen/start_screen.h"
#include "screen/settings_screen.h"
#include "screen/map_editor_screen.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    StartScreen *m_startScreen;
    GameScreen *m_gameScreen;
    SettingsScreen *m_settingsScreen;
    MapEditorScreen *m_mapEditorScreen;
    QStackedWidget *m_stackedWidget;

    void switchScreen(Screen *a, Screen *b);
};

#endif // MAIN_WINDOW_H
