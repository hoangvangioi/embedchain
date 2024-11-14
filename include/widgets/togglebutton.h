#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include "settings.h"
#include "theme.h"
#include "tooltip.h"

#include <QColor>
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QString>

class ToggleButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ToggleButton(QWidget *parent = nullptr, QWidget *app_parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void changeStyle(QEvent::Type event_type);
    void iconPaint(QPainter *qp, const QRect &rect);
    void moveTooltip();

    // Properties
    QWidget *_parent;
    QWidget *_app_parent;
    QString  _tooltip_text;
    QString  _set_icon_path;
    // Color    _params = Theme::color();
    int      _set_radius;
    ToolTip *_tooltip;
};

#endif // TOGGLEBUTTON_H
