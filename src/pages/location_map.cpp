#include "location_map.h"

LocationMap::LocationMap(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("location_map");
    this->setStyleSheet("font-size: 16pt");

    page_map_layout = new QVBoxLayout(this);
    page_map_layout->setSpacing(5);
    page_map_layout->setContentsMargins(5, 5, 5, 5);

    label = new QLabel("Location Map Page Comming Soon", this);
    label->setAlignment(Qt::AlignCenter);
    page_map_layout->addWidget(label);
}

LocationMap::~LocationMap() {}
