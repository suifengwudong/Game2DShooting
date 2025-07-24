#ifndef DEATH_SCREEN_H
#define DEATH_SCREEN_H

#include "screen.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class DeathScreen : public Screen
{
    Q_OBJECT
public:
    explicit DeathScreen(QWidget *parent = nullptr);
    void setMessage(const QString& msg);

signals:
    void gameRestarted();
    void gameExited();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel* messageLabel;
    QPushButton* restartButton;
    QPushButton* exitButton;
};

#endif // DEATH_SCREEN_H
