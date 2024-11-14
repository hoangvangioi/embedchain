#ifndef LOCATION_MAP_H
#define LOCATION_MAP_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

class LocationMap : public QWidget
{
    Q_OBJECT

public:
    explicit LocationMap(QWidget *parent = nullptr);
    ~LocationMap();

private:
    QVBoxLayout *page_map_layout;
    QLabel      *label;
};

#endif // LOCATION_MAP_H
