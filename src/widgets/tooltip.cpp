#include "tooltip.h"

#include "theme.h"

#include <QDebug>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QPainter>

ToolTip::ToolTip(QWidget *parent, const QString &tooltip) : QLabel(parent)
{
    setFont(QFont("Roboto", 9, 800, false));
    setContentsMargins(10, 10, 10, 10);
    setMinimumHeight(34);
    setText(tooltip);
    adjustSize();

    // Set shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 80)); // Dark shadow
    setGraphicsEffect(shadow);

    // Initially hide the tooltip
    hide();
}

void ToolTip::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw the rounded background
    painter.setBrush(QBrush(palette().color(QPalette::Text)));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, width(), height(), 10, 10);

    // Set text color and draw centered text
    painter.setPen(palette().color(QPalette::Window));
    QFontMetrics fm(font());
    QRect        text_rect = fm.boundingRect(text());
    int          x         = (width() - text_rect.width()) / 2;
    int          y         = (height() - text_rect.height()) / 2 + fm.ascent();
    painter.drawText(x, y, text());
}
