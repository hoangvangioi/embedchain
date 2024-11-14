#ifndef TOOLTIP_H
#define TOOLTIP_H

#include <QColor>
#include <QLabel>
#include <QPainter>

class ToolTip : public QLabel
{
    Q_OBJECT

public:
    // Constructor
    explicit ToolTip(QWidget *parent, const QString &tooltip);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // TOOLTIP_H
