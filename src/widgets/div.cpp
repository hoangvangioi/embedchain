#include "div.h"

HDiv::HDiv(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 5, 0, 5);

    QFrame *line = new QFrame(this);
    // line->setBackgroundRole(QPalette::Window);
    line->setAutoFillBackground(true);
    line->setFixedWidth(1);

    hLayout->addWidget(line);

    setFixedWidth(20);
}

VDiv::VDiv(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(5, 0, 5, 0);

    QFrame *line = new QFrame(this);
    // line->setBackgroundRole(QPalette::Window);
    line->setAutoFillBackground(true);
    line->setFixedHeight(1);

    vLayout->addWidget(line);

    setFixedHeight(1);
}
