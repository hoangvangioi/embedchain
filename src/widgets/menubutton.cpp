#include "menubutton.h"

#include <QColor>
#include <QCursor>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QWidget>

MenuButton::MenuButton(QWidget       *app_parent,
                       const QString &text,
                       const QString &btn_id,
                       const QString &tooltip_text,
                       const QString &icon_path,
                       const QString &icon_active_menu,
                       bool           is_active,
                       bool           is_toggle_active)
    : QPushButton(text, app_parent),
      _app_parent(app_parent),
      _tooltip_text(tooltip_text),
      _is_active(is_active),
      _is_toggle_active(is_toggle_active),
      _icon_path(icon_path),
      _icon_active_menu(icon_active_menu),
      _params(Theme::color())
{
    setText(text);
    setMaximumHeight(50);
    setMinimumHeight(50);
    setObjectName(btn_id);

    _tooltip = new ToolTip(app_parent, tooltip_text);
    setCursor(Qt::PointingHandCursor);
}

void MenuButton::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setFont(font());

    QRect rect_inside(4, 5, width() - 8, height() - 10);
    QRect rect_icon(0, 0, 50, height());
    QRect rect_blue(4, 5, 20, height() - 10);
    QRect rect_inside_active(7, 5, width(), height() - 10);
    QRect rect_text(45, 0, width() - 50, height());

    QString icon_path  = _icon_active_menu;
    _params.icon_color = _params.icon_active;

    if (_is_active)
    {
        p.setBrush(_is_active ? _params.context_color : _params.dark_three);
        p.drawRoundedRect(rect_blue, 8, 8);

        p.setBrush(_params.bg_one);
        p.drawRoundedRect(rect_inside_active, 8, 8);

        iconActive(p, icon_path, width());

        p.setPen(_params.text_active);
        p.drawText(rect_text, Qt::AlignVCenter, text());

        iconPaint(p, _icon_path, rect_icon, _params.icon_color);
    }
    else
    {
        p.setBrush(_is_toggle_active ? _params.dark_three : _params.bg_one);
        p.drawRoundedRect(rect_inside, 8, 8);
        p.setPen(_params.text_foreground);
        p.drawText(rect_text, Qt::AlignVCenter, text());
        iconPaint(p, _icon_path, rect_icon, _is_toggle_active ? _params.context_color : _params.icon_color);
    }
    p.end();
}

void MenuButton::enterEvent(QEnterEvent *event)
{
    changeStyle(QEvent::Enter);
    if (width() == 50 && !_tooltip_text.isEmpty())
    {
        moveTooltip();
        _tooltip->show();
    }
}

void MenuButton::leaveEvent(QEvent *event)
{
    changeStyle(QEvent::Leave);
    _tooltip->hide();
}

void MenuButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        changeStyle(QEvent::MouseButtonPress);
        _tooltip->hide();
        emit clicked();
    }
}

void MenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        changeStyle(QEvent::MouseButtonRelease);
        emit released();
    }
}

void MenuButton::changeStyle(QEvent::Type event)
{
    if (event == QEvent::Enter)
    {
        if (!_is_active)
        {
            _params.icon_color = _params.icon_hover;
            _params.bg_one     = _params.dark_three;
        }
    }
    else if (event == QEvent::Leave)
    {
        if (!_is_active)
        {
            _params.icon_color = _params.icon_color;
            _params.bg_one     = _params.dark_one;
        }
    }
    else if (event == QEvent::MouseButtonPress)
    {
        if (!_is_active)
        {
            _params.icon_color = _params.context_color;
            _params.bg_one     = _params.dark_three;
        }
    }
    else if (event == QEvent::MouseButtonRelease)
    {
        if (!_is_active)
        {
            _params.icon_color = _params.icon_hover;
            _params.bg_one     = _params.dark_three;
        }
    }
    repaint();
}

void MenuButton::iconPaint(QPainter &qp, const QString &image, const QRect &rect, const QColor &color)
{
    QPixmap  icon(image);
    QPainter painter(&icon);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(icon.rect(), color);
    qp.drawPixmap((rect.width() - icon.width()) / 2, (rect.height() - icon.height()) / 2, icon);
    painter.end();
}

void MenuButton::iconActive(QPainter &qp, const QString &image, int width)
{
    QPixmap  icon(image);
    QPainter painter(&icon);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(icon.rect(), _params.bg_one);
    qp.drawPixmap(width - 5, 0, icon);
    painter.end();
}

void MenuButton::moveTooltip()
{
    QPoint pos = _app_parent->mapFromGlobal(mapToGlobal(QPoint(0, 0)));
    _tooltip->move(pos.x() + width() + 5, pos.y() + (width() - _tooltip->height()) / 2);
}

bool MenuButton::isActive() const
{
    return _is_active;
}

void MenuButton::setActive(bool is_active)
{
    _is_active = is_active;
    if (!is_active)
    {
        _params.icon_color = _params.icon_color;
        _params.bg_one     = _params.dark_one;
    }
    repaint();
}

void MenuButton::setActiveToggle(bool is_active)
{
    _is_toggle_active = is_active;
}

void MenuButton::setIcon(const QString &icon_path)
{
    _icon_path = icon_path;
    repaint();
}
