#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
{
    Q_UNUSED(parent);
    m_startScreen = new StartScreen(this);
    m_gameScreen = new GameScreen(this);
    m_pauseScreen = new PauseScreen(this);
    m_settingsScreen = new SettingsScreen(this);
    m_mapEditorScreen = new MapEditorScreen(this);
    m_deathScreen = new DeathScreen(this); 

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_startScreen);
    m_stackedWidget->addWidget(m_gameScreen);
    m_stackedWidget->addWidget(m_pauseScreen);
    m_stackedWidget->addWidget(m_settingsScreen);
    m_stackedWidget->addWidget(m_mapEditorScreen);
    m_stackedWidget->addWidget(m_deathScreen);

    setCentralWidget(m_stackedWidget);
    setFixedSize(m_startScreen->size());

    connect(m_startScreen, &StartScreen::startGameClicked, [this]{
        switchScreen(m_startScreen, m_gameScreen);
        m_gameScreen->initGame();
    });
    connect(m_startScreen, &StartScreen::settingsClicked, [this]{
        switchScreen(m_startScreen, m_settingsScreen);
    });
    connect(m_startScreen, &StartScreen::mapEditorClicked, [this]{
        switchScreen(m_startScreen, m_mapEditorScreen);
    });
    connect(m_settingsScreen, &SettingsScreen::backToStartScreen, [this]{
        switchScreen(m_settingsScreen, m_startScreen);
    });
    connect(m_mapEditorScreen, &MapEditorScreen::backToStartScreen, [this]{
        switchScreen(m_mapEditorScreen, m_startScreen);
    });
    connect(m_gameScreen, &GameScreen::gamePaused, [this](QStringList statuslist){
        switchScreen(m_gameScreen, m_pauseScreen);
        m_pauseScreen->setPlayersStatus(statuslist);
    });
    connect(m_gameScreen, &GameScreen::gameEnd, [this](QString &playerName){
        m_deathScreen->setMessage(QString("%1 已死亡！").arg(playerName));
        switchScreen(m_gameScreen, m_deathScreen);
    });
    connect(m_pauseScreen, &PauseScreen::gameResumed, [this]{
        switchScreen(m_pauseScreen, m_gameScreen);
        m_gameScreen->resumeGame();
    });
    connect(m_deathScreen, &DeathScreen::gameRestarted, [this]{
        switchScreen(m_deathScreen, m_gameScreen);
        m_gameScreen->initGame();
    });
    connect(m_deathScreen, &DeathScreen::gameExited, [this]{
        // 退出程序
        close(); 
    });
}

MainWindow::~MainWindow()
{
    if (m_startScreen) delete m_startScreen;
    if (m_gameScreen) delete m_gameScreen;
    if (m_pauseScreen) delete m_pauseScreen;
    if (m_settingsScreen) delete m_settingsScreen;
    if (m_mapEditorScreen) delete m_mapEditorScreen;
    if (m_deathScreen) delete m_deathScreen;
    if (m_stackedWidget) delete m_stackedWidget;
}

void MainWindow::switchScreen(Screen *a, Screen *b) {
    if (a && b) {
        m_stackedWidget->setCurrentWidget(b);
        setFixedSize(b->size());
    }
}
