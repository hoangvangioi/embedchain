#include "analytics.h"

#include <QChartView>
#include <QLineSeries>
#include <QMessageBox>
#include <QValueAxis>

Analytics::Analytics(QWidget *parent) : QWidget(parent), chart(nullptr), timer(new QTimer(this))
{
    layout    = new QVBoxLayout(this);
    comboBox  = new QComboBox(this);
    chartView = new QChartView(this);

    // Set up the combo box for selecting the data type
    comboBox->addItem("Temperature");
    comboBox->addItem("Humidity");
    comboBox->addItem("Pressure");
    comboBox->addItem("CO2");
    comboBox->addItem("NO2");
    comboBox->addItem("O3");

    layout->addWidget(comboBox);
    layout->addWidget(chartView);

    currentPlotType = "Temperature";
    comboBox->setCurrentIndex(0);

    connect(comboBox, &QComboBox::currentIndexChanged, this, &Analytics::onComboBoxChanged);
    connect(timer, &QTimer::timeout, this, &Analytics::fetchDataAndPlot);
    timer->start(2000);
}

Analytics::~Analytics() {}

void Analytics::onComboBoxChanged(int index)
{
    currentPlotType = comboBox->currentText();
    fetchDataAndPlot();
}

bool Analytics::fetchData(const QString &plotType, QMap<QString, QVector<QPointF>> &dataMap)
{
    QString query = QString("SELECT timestamp, %1, latitude, longitude FROM data ORDER BY timestamp DESC LIMIT 300").arg(plotType.toLower());

    sqlite3_stmt *stmt;
    int           rc = sqlite3_prepare_v2(db, query.toUtf8(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        QMessageBox::critical(this, "SQL Error", QString("SQL Error: %1\nQuery: %2").arg(sqlite3_errmsg(db), query));
        return false;
    }

    pthread_mutex_lock(&db_mutex);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        long long timestamp = sqlite3_column_int64(stmt, 0);
        QString   value     = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
        QString   lat       = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
        QString   lon       = QString::fromUtf8(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));

        // Optional: Validate latitude/longitude for numeric values
        bool   latOk, lonOk;
        double latitude  = lat.toDouble(&latOk);
        double longitude = lon.toDouble(&lonOk);

        if (latOk && lonOk)
        {
            QString latLongKey = lat + "," + lon;
            bool    ok;
            double  numericValue = value.toDouble(&ok);
            if (ok)
            {
                dataMap[latLongKey].append(QPointF(timestamp, numericValue));
            }
            else
            {
                qWarning() << "Invalid data value for" << latLongKey << ": " << value;
            }
        }
    }

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return true;
}

void Analytics::plotData(const QMap<QString, QVector<QPointF>> &dataMap)
{
    if (chart)
    {
        // Remove all existing series from the chart
        for (auto series : chart->series())
        {
            chart->removeSeries(series);
        }
    }

    chart = new QChart();

    // Create new series and add them to the chart
    for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
    {
        QString          latLong    = it.key();
        QVector<QPointF> dataPoints = it.value();

        QLineSeries *series = new QLineSeries();
        series->setName(latLong);

        for (const auto &point : dataPoints)
        {
            series->append(point);
        }

        chart->addSeries(series);
    }

    // Create time axis for X-axis (QDateTimeAxis)
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("HH:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);

    // Create value axis for Y-axis
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);

    // Attach axes to all series
    for (auto series : chart->series())
    {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }

    chart->setTitle("Data Chart");
    chartView->setChart(chart);
}

void Analytics::fetchDataAndPlot()
{
    QMap<QString, QVector<QPointF>> dataMap;
    if (fetchData(currentPlotType, dataMap))
    {
        plotData(dataMap);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to fetch data for plot.");
    }
}
