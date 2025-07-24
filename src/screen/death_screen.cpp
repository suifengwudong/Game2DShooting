#include "death_screen.h"
#include <QPainter>
#include <QFont>
#include <QPaintEvent>
#include <QVBoxLayout>

DeathScreen::DeathScreen(QWidget *parent)
    : Screen(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    QWidget* glassPanel = new QWidget(this);
    glassPanel->setObjectName("glassPanel");
    glassPanel->setStyleSheet(
        "#glassPanel {"
        "  background: rgba(30, 30, 40, 0.82);"
        "  border-radius: 36px;"
        "  border: 2px solid rgba(255,255,255,0.12);"
        "}"
    );

    QVBoxLayout* glassLayout = new QVBoxLayout(glassPanel);
    glassLayout->setContentsMargins(60, 48, 60, 48);
    glassLayout->setSpacing(36);

    // 死亡大标题
    messageLabel = new QLabel(QStringLiteral("你已死亡！"), glassPanel);
    QFont font = messageLabel->font();
    font.setPointSize(56);
    font.setBold(true);
    messageLabel->setFont(font);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet(
        "color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ffb3b3);"
        "letter-spacing: 8px;"
        "border-bottom: 3px solid #ff4444;"
        "padding-bottom: 12px;"
    );
    glassLayout->addWidget(messageLabel);

    // 按钮区
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setSpacing(48);
    restartButton = new QPushButton(QStringLiteral("重新开始"), glassPanel);
    restartButton->setFixedSize(240, 60);
    restartButton->setProperty("deathscreenRole", "restart");
    connect(restartButton, &QPushButton::clicked, this, &DeathScreen::gameRestarted);
    restartButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 28px; font-weight: bold; letter-spacing: 2px;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #44e944, stop:1 #228822);"
        "  color: white; border-radius: 30px; border: 2px solid #fff;"
        "  padding: 8px 32px;"
        "}"
        "QPushButton:hover { background: #66ff66; color: #222; }"
        "QPushButton:pressed { background: #228822; }"
    );
    btnRow->addStretch(1);
    btnRow->addWidget(restartButton);

    exitButton = new QPushButton(QStringLiteral("退出游戏"), glassPanel);
    exitButton->setFixedSize(240, 60);
    exitButton->setProperty("deathscreenRole", "exit");
    connect(exitButton, &QPushButton::clicked, this, &DeathScreen::gameExited);
    exitButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 28px; font-weight: bold; letter-spacing: 2px;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ff4444, stop:1 #882222);"
        "  color: white; border-radius: 30px; border: 2px solid #fff;"
        "  padding: 8px 32px;"
        "}"
        "QPushButton:hover { background: #ff8888; color: #222; }"
        "QPushButton:pressed { background: #882222; }"
    );
    btnRow->addWidget(exitButton);
    btnRow->addStretch(1);
    glassLayout->addLayout(btnRow);

    // 主布局居中毛玻璃面板
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch(1);
    mainLayout->addWidget(glassPanel, 0, Qt::AlignHCenter);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void DeathScreen::setMessage(const QString& msg)
{
    messageLabel->setText(QString("<span style='font-size:32px; color:#fff;'>%1</span>").arg(msg));
}

void DeathScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 渐变黑色背景
    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0, QColor(10, 10, 20, 220));
    bg.setColorAt(1, QColor(40, 0, 0, 220));
    painter.fillRect(rect(), bg);
}
