#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
{
    Q_UNUSED(parent);
    m_startScreen = new StartScreen(this);
    m_gameScreen = new GameScreen(this);
    m_settingsScreen = new SettingsScreen(this);
    m_mapEditorScreen = new MapEditorScreen(this);

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_startScreen);
    m_stackedWidget->addWidget(m_gameScreen);
    m_stackedWidget->addWidget(m_settingsScreen);
    m_stackedWidget->addWidget(m_mapEditorScreen);

    setCentralWidget(m_stackedWidget);
    setFixedSize(m_startScreen->size());

    connect(m_startScreen, &StartScreen::startGameClicked, [this]{
        switchScreen(m_startScreen, m_gameScreen);
        m_gameScreen->initGame();
    });
    connect(m_startScreen, &StartScreen::settingsClicked, [this]{
        switchScreen(m_startScreen, m_settingsScreen);
    });
    connect(m_settingsScreen, &SettingsScreen::backToStartScreen, [this]{
        switchScreen(m_settingsScreen, m_startScreen);
    });
    connect(m_startScreen, &StartScreen::mapEditorClicked, [this]{
        switchScreen(m_startScreen, m_mapEditorScreen);
    });
    connect(m_mapEditorScreen, &MapEditorScreen::backToStartScreen, [this]{
        switchScreen(m_mapEditorScreen, m_startScreen);
    });
}

MainWindow::~MainWindow()
{
    if (m_startScreen) delete m_startScreen;
    if (m_gameScreen) delete m_gameScreen;
    if (m_settingsScreen) delete m_settingsScreen;
    if (m_stackedWidget) delete m_stackedWidget;
}

void MainWindow::switchScreen(Screen *a, Screen *b) {
    if (a && b) {
        m_stackedWidget->setCurrentWidget(b);
        setFixedSize(b->size());
    }
}
