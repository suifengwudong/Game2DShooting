#include "start_screen.h"
#include "ui_start_screen.h"
#include "config.h"

StartScreen::StartScreen(QWidget *parent) :
    Screen(parent), ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    // 优化窗口大小
    setFixedSize(QSize(START_WIDTH, START_HEIGHT));
    setStyleSheet(
        "QWidget#StartScreen {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e3f0ff, stop:1 #f9fcff);"
        "}"
        "QLabel#label {"
        "  font-size: 48px; font-weight: bold; color: #2a4d7a; letter-spacing: 2px;"
        "  margin-top: 32px; margin-bottom: 32px;"
        "}"
        "QPushButton {"
        "  font-size: 22px; font-weight: 600; border-radius: 22px; padding: 12px 0;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #6ec1e4, stop:1 #367dc4);"
        "  color: white; margin: 12px 0;"
        "}"
        "QPushButton:hover { background: #5dbbe3; }"
        "QPushButton:pressed { background: #367dc4; }"
    );
    // 按钮高度统一
    ui->startButton->setMinimumHeight(64);
    ui->settingsButton->setMinimumHeight(64);
    ui->mapEditorButton->setMinimumHeight(64);
    ui->exitButton->setMinimumHeight(64);
    ui->startButton->setMinimumWidth(220);
    ui->settingsButton->setMinimumWidth(220);
    ui->mapEditorButton->setMinimumWidth(220);
    ui->exitButton->setMinimumWidth(220);

    connect(ui->startButton, &QPushButton::clicked, [this]{
        emit startGameClicked();
    });
    connect(ui->exitButton, &QPushButton::clicked, [this] {
        qApp->quit();
    });
    connect(ui->settingsButton, &QPushButton::clicked, [this] {
        emit settingsClicked();
    });
    connect(ui->mapEditorButton, &QPushButton::clicked, [this] {
        emit mapEditorClicked();
    });
}

StartScreen::~StartScreen() {
    delete ui;
}


