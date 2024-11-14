#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "database.h"
#include "sqlite3.h"

#include <QComboBox>
#include <QLabel>
#include <QMap>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCharts>

class Analytics : public QWidget
{
    Q_OBJECT

public:
    explicit Analytics(QWidget *parent = nullptr);
    ~Analytics();

private slots:
    void onComboBoxChanged(int index);
    void fetchDataAndPlot();

private:
    QVBoxLayout *layout;
    QComboBox   *comboBox;
    QChartView  *chartView;
    QChart      *chart;

    sqlite3 *db = global_db;
    QString  currentPlotType;
    QTimer  *timer;

    bool fetchData(const QString &plotType, QMap<QString, QVector<QPointF>> &dataMap);
    void plotData(const QMap<QString, QVector<QPointF>> &dataMap);
};

#endif // ANALYTICS_H
