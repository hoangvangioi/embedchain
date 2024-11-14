#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "database.h"
#include "sqlite3.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

// Định nghĩa cấu trúc dữ liệu để lưu trữ các giá trị sensor
typedef struct
{
    double temperature;
    double humidity;
    double pressure;
    double co2;
    double no2;
    double o3;
    char   latitude[20];
    char   longitude[20];
} SensorData;

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:
    void updateSensorDataFromDb();

private:
    sqlite3     *db = global_db;
    QVBoxLayout *layout;
    QScrollArea *scrollArea;
    QWidget     *contentWidget;
    QGridLayout *gridLayout;

    QVector<QLabel *>   labels_4, labels_5, labels_23, location;
    QVector<QFrame *>   frames;
    QVector<SensorData> sensorData;

    QFrame      *frame;
    QVBoxLayout *verticalLayout;

    QLabel *label, *label_9, *label_13;

    QPushButton *pushButton_2;
    QLabel      *widget_16;

    QLabel      *label_6;
    QFrame      *frame_17;
    QHBoxLayout *horizontalLayout;
    QWidget     *widget_8;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_5;

    QLabel      *widget;
    QVBoxLayout *verticalLayout_7;
    QFrame      *frame_6;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;

    QLabel      *widget_2;
    QVBoxLayout *verticalLayout_6;

    QHBoxLayout *horizontalLayout_2;

    QLabel      *widget_3;
    QVBoxLayout *verticalLayout_5;

    const int numFrames = 5;

    void updateLabels();
    void queryLatestDataForCoordinates();
};

#endif // HOMEPAGE_H
