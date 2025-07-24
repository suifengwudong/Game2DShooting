#include "pause_screen.h"
#include <QPainter>
#include <QFont>
#include <QPaintEvent>
#include <QVBoxLayout>

PauseScreen::PauseScreen(QWidget *parent)
    : Screen(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    // 主布局：分为上中下三部分
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(60, 40, 60, 40);
    mainLayout->setSpacing(0);

    // 顶部空间（标题区，paintEvent绘制）
    QWidget* titleSpacer = new QWidget(this);
    titleSpacer->setFixedHeight(90);
    mainLayout->addWidget(titleSpacer);

    // 中部：玩家状态卡片
    QHBoxLayout* centerRow = new QHBoxLayout();
    centerRow->addStretch(1);
    playerStatusList = new QListWidget(this);
    playerStatusList->setStyleSheet(
        "QListWidget {"
        "  background: rgba(255,255,255,220);"
        "  border-radius: 18px;"
        "  font-size: 19px;"
        "  padding: 16px;"
        "  border: 2px solid #e0e0e0;"
        "}"
        "QListWidget::item {"
        "  margin: 10px;"
        "  padding: 10px 20px;"
        "  border-radius: 10px;"
        "  background: #f7fafd;"
        "  color: #333;"
        "  border: 1px solid #d0e6f7;"
        "}"
        "QListWidget::item:selected {"
        "  background: #aee1f9;"
        "  color: #222;"
        "}"
    );
    playerStatusList->setFixedHeight(240);
    playerStatusList->setMinimumWidth(340);
    centerRow->addWidget(playerStatusList, 10);
    centerRow->addStretch(1);
    mainLayout->addLayout(centerRow, 10);

    // 底部：按钮区
    QWidget* btnSpacer = new QWidget(this);
    btnSpacer->setFixedHeight(30);
    mainLayout->addWidget(btnSpacer);
    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->addStretch(1);
    resumeButton = new QPushButton(QStringLiteral("▶ 继续游戏"), this);
    resumeButton->setFixedSize(240, 54);
    resumeButton->setProperty("pausescreenRole", "resume");
    connect(resumeButton, &QPushButton::clicked, [this]{
        emit gameResumed();
    });
    resumeButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 24px; font-weight: bold; letter-spacing: 2px;"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #44cc44, stop:1 #228822);"
        "  color: white; border-radius: 27px; border: 2px solid #fff;"
        "  padding: 8px 32px;"
        "}"
        "QPushButton:hover { background: #66dd66; color: #222; }"
        "QPushButton:pressed { background: #228822; }"
    );
    btnRow->addWidget(resumeButton, 0, Qt::AlignHCenter);
    btnRow->addStretch(1);
    mainLayout->addLayout(btnRow, 0);
}

void PauseScreen::setPlayersStatus(QStringList& statusList)
{
    playerStatusList->clear();
    for (QString &status : statusList) {
        playerStatusList->addItem(status);
    }
}

void PauseScreen::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit gameResumed(); // 发送游戏恢复的信号
    }
    Screen::keyPressEvent(event);
}

void PauseScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // 半透明黑色遮罩
    painter.fillRect(rect(), QColor(0, 0, 0, 160));
    // 居中大字带阴影和渐变
    QFont font = painter.font();
    font.setPointSize(54);
    font.setBold(true);
    painter.setFont(font);
    QRect titleRect(0, 30, width(), 80);
    // 渐变色
    QLinearGradient grad(0, 30, width(), 110);
    grad.setColorAt(0, QColor(255,255,255));
    grad.setColorAt(1, QColor(100,180,255));
    painter.setPen(Qt::NoPen);
    painter.setBrush(grad);
    painter.drawRect(titleRect);
    // 阴影
    painter.setPen(QColor(0,0,0,120));
    painter.drawText(titleRect.translated(2,4), Qt::AlignCenter, QStringLiteral("游戏暂停"));
    // 白色描边大字
    painter.setPen(QColor(30,60,180));
    painter.drawText(titleRect, Qt::AlignCenter, QStringLiteral("游戏暂停"));
}
