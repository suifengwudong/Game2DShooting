#include "config.h"
#include "settings_screen.h"
#include "ui_settings_screen.h"

SettingsScreen::SettingsScreen(QWidget *parent) :
    Screen(parent), ui(new Ui::SettingsScreen)
{
    ui->setupUi(this);
    setFixedSize(QSize(SETTINGS_WIDTH, SETTINGS_HEIGHT));

    connect(ui->saveButton, &QPushButton::clicked, [this]{
        //
    });
    connect(ui->retreatButton, &QPushButton::clicked, [this]{
        emit backToStartScreen();
    });
}

SettingsScreen::~SettingsScreen()
{
    // delete ui;
}
