#include "leftmenu.h"

#include <QEasingCurve>
#include <QFrame>
#include <QIcon>
#include <QMap>
#include <QPainter>
#include <QPropertyAnimation>
#include <QStyle>
#include <QStyleOptionButton>
#include <QVBoxLayout>
#include <QVariant>

LeftMenu::LeftMenu(QWidget       *parent,
                   QWidget       *appParent,
                   const QString &iconPath,
                   const QString &iconPathClose,
                   const QString &toggleText,
                   const QString &toggleTooltip,
                   int            minimumWidth,
                   int            maximumWidth)
    : QWidget(parent), _parent(parent), _appParent(appParent), _iconPath(iconPath), _iconPathClose(iconPathClose), _minimumWidth(minimumWidth), _maximumWidth(maximumWidth)

{
    setupUI();

    // Toggle button
    toggleButton = new MenuButton(_appParent, toggleText, "", toggleTooltip, QString("%1").arg(_iconPath));
    connect(toggleButton, &MenuButton::clicked, this, &LeftMenu::toggleAnimation);

    // Divs
    divTop    = new VDiv();
    divBottom = new VDiv();
    divBottom->hide();

    // Layouts
    topLayout->addWidget(toggleButton);
    topLayout->addWidget(divTop);

    bottomLayout->addWidget(divBottom);
}

void LeftMenu::addMenus(const QList<QMap<QString, QVariant>> &parameters)
{
    for (const auto &param : parameters)
    {
        QString btnIcon    = param["btn_icon"].toString();
        QString btnId      = param["btn_id"].toString();
        QString btnText    = param["btn_text"].toString();
        QString btnTooltip = param["btn_tooltip"].toString();
        bool    showTop    = param["show_top"].toBool();
        bool    isActive   = param["is_active"].toBool();

        menuButton = new MenuButton(_appParent, btnText, btnId, btnTooltip, QString(":/icons/%1").arg(btnIcon), ":/icons/active_menu.svg", isActive);

        connect(menuButton, &MenuButton::clicked, this, &LeftMenu::btnClicked);
        connect(menuButton, &MenuButton::released, this, &LeftMenu::btnReleased);

        if (showTop)
        {
            topLayout->addWidget(menuButton);
        }
        else
        {
            divBottom->show();
            bottomLayout->addWidget(menuButton);
        }
    }
}

void LeftMenu::btnClicked()
{
    emit clicked(qobject_cast<QPushButton *>(sender()));
}
void LeftMenu::btnReleased()
{
    emit released(qobject_cast<QPushButton *>(sender()));
}

void LeftMenu::toggleAnimation()
{
    animation = new QPropertyAnimation(_parent, "minimumWidth");
    animation->stop();
    if (width() == _minimumWidth)
    {
        animation->setStartValue(width());
        animation->setEndValue(_maximumWidth);
        toggleButton->setActiveToggle(true);
        toggleButton->setIcon(_iconPathClose);
    }
    else
    {
        animation->setStartValue(width());
        animation->setEndValue(_minimumWidth);
        toggleButton->setActiveToggle(false);
        toggleButton->setIcon(_iconPath);
    }
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->setDuration(300);
    animation->start();
}

void LeftMenu::setupUI()
{
    leftMenuLayout = new QVBoxLayout(this);
    leftMenuLayout->setContentsMargins(0, 0, 0, 0);

    bg          = new QFrame();
    topFrame    = new QFrame();
    bottomFrame = new QFrame();

    // Layouts
    layout = new QVBoxLayout(bg);
    layout->setContentsMargins(0, 0, 0, 0);

    topLayout = new QVBoxLayout(topFrame);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(1);

    bottomLayout = new QVBoxLayout(bottomFrame);
    bottomLayout->setContentsMargins(0, 0, 0, 8);
    bottomLayout->setSpacing(1);

    // Add frames
    layout->addWidget(topFrame, 0, Qt::AlignTop);
    layout->addWidget(bottomFrame, 0, Qt::AlignBottom);

    leftMenuLayout->addWidget(bg);
}

void LeftMenu::selectOnlyOne(const QString &widgetName)
{
    QList<MenuButton *> buttons = findChildren<MenuButton *>();
    for (MenuButton *btn : buttons)
    {
        if (btn->objectName() == widgetName)
        {
            btn->setActive(true);
        }
        else
        {
            btn->setActive(false);
        }
    }
}

void LeftMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // painter.setBrush(palette().color(QPalette::WindowText));
    painter.drawRoundedRect(rect(), 8, 8);
}
