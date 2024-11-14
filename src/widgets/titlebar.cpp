#include "titlebar.h"

#include <QColor>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

TitleBar::TitleBar(QWidget *parent, QWidget *app_parent, const QString &logo_image, int logo_width, int radius, const QString &font_family, int title_size)
    : QWidget(parent), _parent(parent), _app_parent(app_parent), _radius(radius), _font_family(font_family), _title_size(title_size)
{
    setupUi();

    top_logo->setFixedWidth(logo_width);

    bg_layout->addWidget(top_logo);
    bg_layout->addWidget(div_1);
    bg_layout->addWidget(title_label);
    bg_layout->addWidget(div_2);
    bg_layout->addLayout(custom_buttons_layout);
}

void TitleBar::setupUi()
{
    // Set up the layout for the title bar
    QVBoxLayout *title_bar_layout = new QVBoxLayout(this);
    title_bar_layout->setContentsMargins(0, 0, 0, 0);

    bg        = new QFrame(this);
    bg_layout = new QHBoxLayout(bg);
    bg_layout->setContentsMargins(10, 0, 10, 0);
    bg_layout->setSpacing(0);

    div_1 = new HDiv();
    div_2 = new HDiv();

    // Set up the left side (logo)
    top_logo = new QLabel(this);
    top_logo->setTextFormat(Qt::TextFormat::MarkdownText);
    top_logo->setText("## Hello World! - LOGO");

    // Set up the title label
    title_label = new QLabel(this);
    title_label->setAlignment(Qt::AlignVCenter);
    title_label->setStyleSheet(QString("font: %1pt \"%2\"; color: blue;").arg(_title_size).arg(_font_family));

    // Set up the custom buttons layout
    custom_buttons_layout = new QHBoxLayout();
    custom_buttons_layout->setContentsMargins(0, 0, 0, 0);
    custom_buttons_layout->setSpacing(3);

    menu = new ToggleButton(_parent, _app_parent);

    connect(menu, &ToggleButton::clicked, this, &TitleBar::btnClicked);
    connect(menu, &ToggleButton::released, this, &TitleBar::btnReleased);

    custom_buttons_layout->addWidget(menu);

    // Add all widgets to the layout
    title_bar_layout->addWidget(bg);
}

void TitleBar::btnClicked()
{
    emit clicked(qobject_cast<QPushButton *>(sender()));
}

void TitleBar::btnReleased()
{
    emit released(qobject_cast<QPushButton *>(sender()));
}

void TitleBar::setTitle(const QString &title)
{
    title_label->setText(title);
}

void TitleBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // painter.setBrush(palette().color(QPalette::WindowText));
    painter.drawRoundedRect(rect(), 8, 8);
}
