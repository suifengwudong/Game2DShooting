#ifndef PAUSE_SCREEN_H
#define PAUSE_SCREEN_H

#include "screen.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

class PauseScreen : public Screen 
{
    Q_OBJECT
public:
    explicit PauseScreen(QWidget *parent = nullptr);
    void setPlayersStatus(QStringList& statusList);
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void gameResumed();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton* resumeButton;
    QListWidget* playerStatusList;
};

#endif // PAUSE_SCREEN_H
