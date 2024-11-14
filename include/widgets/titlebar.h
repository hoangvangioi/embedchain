#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "div.h"
#include "settings.h"
#include "theme.h"
#include "togglebutton.h"

#include <QColor>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include <QString>
#include <QSvgWidget>
#include <QVBoxLayout>
#include <QWidget>

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget       *parent,
                      QWidget       *app_parent,
                      const QString &logo_image  = "logo_top_100x22.svg",
                      int            logo_width  = 180,
                      int            radius      = 8,
                      const QString &font_family = "Roboto",
                      int            title_size  = 12);

    void setTitle(const QString &title);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void clicked(QWidget *sender);
    void released(QWidget *sender);

private slots:
    void btnClicked();
    void btnReleased();

private:
    void setupUi();

    QWidget *_parent;
    QWidget *_app_parent;

    int     _radius;
    QString _font_family;
    int     _title_size;

    QFrame       *bg;
    QHBoxLayout  *bg_layout;
    HDiv         *div_1, *div_2;
    QLabel       *top_logo;
    QVBoxLayout  *top_logo_layout;
    QSvgWidget   *logo_svg;
    QLabel       *title_label;
    QHBoxLayout  *custom_buttons_layout;
    ToggleButton *menu;
};

#endif // TITLEBAR_H
