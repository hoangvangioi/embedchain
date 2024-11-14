#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "settings.h"
#include "theme.h"
#include "tooltip.h"

#include <QDir>
#include <QEvent>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QString>
#include <QWidget>

class MenuButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MenuButton(QWidget       *app_parent,
                        const QString &text,
                        const QString &btn_id           = QString(),
                        const QString &tooltip_text     = "",
                        const QString &icon_path        = ":/icons/add_user.svg",
                        const QString &icon_active_menu = ":/icons/active_menu.svg",
                        bool           is_active        = false,
                        bool           is_toggle_active = false);

    void setActive(bool is_active);
    bool isActive() const;
    void setActiveToggle(bool is_active);
    void setIcon(const QString &icon_path);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void changeStyle(QEvent::Type event);
    void iconPaint(QPainter &qp, const QString &image, const QRect &rect, const QColor &color);
    void iconActive(QPainter &qp, const QString &image, int width);
    void moveTooltip();

    QWidget *_app_parent;
    bool     _is_active;
    bool     _is_toggle_active;

    QString _tooltip_text;
    QString _icon_path;
    QString _icon_active_menu;

    Color _params;

    ToolTip *_tooltip;
};

#endif // MENUBUTTON_H
