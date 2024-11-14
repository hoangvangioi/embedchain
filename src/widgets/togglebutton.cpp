#include "togglebutton.h"

ToggleButton::ToggleButton(QWidget *parent, QWidget *app_parent) : QPushButton(parent), _parent(parent), _app_parent(app_parent), _set_icon_path(":/icons/moon.svg")
{
    setFixedSize(30, 30);
    setCursor(Qt::PointingHandCursor);
    setObjectName("btn_toggle_theme");

    _tooltip = new ToolTip(app_parent, "Toggle Theme");
}

void ToggleButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    // painter.setBrush(QColor(_params.bg_one));
    painter.setBrush(palette().color(QPalette::WindowText));

    QRect rect(0, 0, width(), height());
    painter.drawRoundedRect(rect, 8, 8);

    iconPaint(&painter, rect);
}

void ToggleButton::enterEvent(QEnterEvent *event)
{
    changeStyle(QEvent::Enter);
    moveTooltip();
    _tooltip->show();
}

void ToggleButton::leaveEvent(QEvent *event)
{
    changeStyle(QEvent::Leave);
    moveTooltip();
    _tooltip->hide();
}

void ToggleButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        changeStyle(QEvent::MouseButtonPress);
        setFocus();
        update();
        emit clicked();
    }
}

void ToggleButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        changeStyle(QEvent::MouseButtonRelease);
        emit released();
    }
}

void ToggleButton::changeStyle(QEvent::Type event_type)
{
    switch (event_type)
    {
        case QEvent::Enter:
            // _params.bg_one     = _params.bg_three;
            // _params.icon_color = _params.icon_hover;
            break;
        case QEvent::Leave:
            // _params.bg_one     = _params.bg_one;
            // _params.icon_color = _params.icon_color;
            break;
        case QEvent::MouseButtonPress:
            // _params.bg_one     = _params.bg_three;
            // _params.icon_color = _params.icon_pressed;
            _set_icon_path = (Theme::instance().getCurrentTheme() == "light") ? ":/icons/moon.svg" : ":/icons/sun.svg";
            break;
        case QEvent::MouseButtonRelease:
            // _params.bg_one     = _params.bg_three;
            // _params.icon_color = _params.icon_hover;
            break;
        default:
            break;
    }
    update();
}

void ToggleButton::iconPaint(QPainter *qp, const QRect &rect)
{
    QPixmap  icon(_set_icon_path);
    QPainter painter(&icon);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    // painter.fillRect(icon.rect(), QColor(_params.icon_color));

    qp->drawPixmap((rect.width() - icon.width()) / 2, (rect.height() - icon.height()) / 2, icon);
}

void ToggleButton::moveTooltip()
{
    QPoint pos = _parent->mapFromGlobal(mapToGlobal(QPoint(0, 0)));
    _tooltip->move(pos.x() - _tooltip->width() + width() + 5, pos.y() + height() + 6);
}