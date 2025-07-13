#include "start_screen.h"
#include "ui_start_screen.h"
#include "config.h"

StartScreen::StartScreen(QWidget *parent) :
    Screen(parent), ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    setFixedSize(QSize(START_WIDTH, START_HEIGHT));

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


