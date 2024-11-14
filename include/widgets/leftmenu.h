#ifndef LEFTMENU_H
#define LEFTMENU_H

#include "div.h"
#include "menubutton.h"
#include "settings.h"
#include "theme.h"

#include <QEasingCurve>
#include <QFrame>
#include <QIcon>
#include <QList>
#include <QMap>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class LeftMenu : public QWidget
{
    Q_OBJECT

public:
    explicit LeftMenu(QWidget       *parent        = nullptr,
                      QWidget       *appParent     = nullptr,
                      const QString &iconPath      = ":/icons/menu.svg",
                      const QString &iconPathClose = ":/icons/arow.svg",
                      const QString &toggleText    = "Hide Menu",
                      const QString &toggleTooltip = "Show menu",
                      int            minimumWidth  = 50,
                      int            maximumWidth  = 240);

    void addMenus(const QList<QMap<QString, QVariant>> &parameters);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void clicked(QWidget *sender);
    void released(QWidget *sender);

public slots:
    void btnClicked();
    void btnReleased();
    void toggleAnimation();

    void selectOnlyOne(const QString &widgetName);

private:
    void setupUI();

    QWidget *_parent;
    QWidget *_appParent;

    int     _minimumWidth, _maximumWidth;
    QString _iconPath, _iconPathClose;

    QVBoxLayout *leftMenuLayout;
    QVBoxLayout *layout;
    QVBoxLayout *topLayout;
    QVBoxLayout *bottomLayout;

    QFrame             *bg, *topFrame, *bottomFrame;
    MenuButton         *toggleButton;
    MenuButton         *menuButton;
    VDiv               *divTop, *divBottom;
    Color               _params;
    QPropertyAnimation *animation;
};

#endif // LEFTMENU_H
