#include "data_history.h"

#include "chacha20.h"
#include "database.h"
#include "utils.h"

#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <stdint.h>
#include <stdio.h>

DataHistory::DataHistory(QWidget* parent)
    : QWidget(parent), tableWidget(new QTableWidget(this)), reloadButton(new QPushButton("Reload", this)), filterButton(new QPushButton("Apply Filter", this))
{

    this->setObjectName("data_history");
    this->setStyleSheet("font-size: 10pt");

    layout = new QVBoxLayout(this);
    layout->addWidget(reloadButton);

    // Layout cho filter
    QHBoxLayout* filterLayout = new QHBoxLayout();
    locationFilter            = new QComboBox(this);
    filterLayout->addWidget(locationFilter);
    filterLayout->addWidget(filterButton);

    layout->addLayout(filterLayout);
    layout->addWidget(tableWidget);

    // Đảm bảo bảng mở rộng và phủ hết widget ngoài
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidget->setColumnCount(8);
    tableWidget->setHorizontalHeaderLabels({"Transaction ID", "Timestamp", "Temperature", "Humidity", "Pressure", "CO2", "NO2", "O3"});

    connect(reloadButton, &QPushButton::clicked, this, &DataHistory::onReloadButtonClicked);
    connect(filterButton, &QPushButton::clicked, this, &DataHistory::onFilterButtonClicked);

    createTableFromDatabase(db, tableWidget);
}

DataHistory::~DataHistory() {}

void DataHistory::onReloadButtonClicked()
{
    createTableFromDatabase(db, tableWidget);
}

bool DataHistory::createTableFromDatabase(sqlite3* db, QTableWidget* tableWidget)
{
    const char*   sql = "SELECT id, timestamp, latitude, longitude, temperature, humidity, pressure, co2, no2, o3 FROM data";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "Failed to fetch data from database:" << sqlite3_errmsg(db);
        return false;
    }

    // Lấy các cặp latitude và longitude
    QSet<QString> locations;

    // Xóa dữ liệu hiện tại trong QTableWidget
    tableWidget->setRowCount(0);

    pthread_mutex_lock(&db_mutex);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        // Retrieve data from the query
        int         id          = sqlite3_column_int(stmt, 0);
        long long   timestamp   = sqlite3_column_int64(stmt, 1);
        const char* latitude    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* longitude   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* temperature = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* humidity    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* pressure    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        const char* co2         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        const char* no2         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        const char* o3          = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));

        // Kết hợp latitude và longitude thành một cặp và thêm vào set
        QString location = QString("%1, %2").arg(latitude).arg(longitude);
        locations.insert(location);

        // Thêm dữ liệu vào các cột trong bảng
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(timestamp).toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(temperature));
        tableWidget->setItem(row, 3, new QTableWidgetItem(humidity));
        tableWidget->setItem(row, 4, new QTableWidgetItem(pressure));
        tableWidget->setItem(row, 5, new QTableWidgetItem(co2));
        tableWidget->setItem(row, 6, new QTableWidgetItem(no2));
        tableWidget->setItem(row, 7, new QTableWidgetItem(o3));
    }

    // Điền các cặp location vào QComboBox
    locationFilter->clear();
    locationFilter->addItems(locations.values());

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return true;
}

void DataHistory::onFilterButtonClicked()
{
    // Lấy giá trị filter từ QComboBox
    QString selectedLocation = locationFilter->currentText().trimmed();

    // Tách latitude và longitude từ chuỗi cặp
    QStringList latLong = selectedLocation.split(", ");
    if (latLong.size() == 2)
    {
        QString latitude  = latLong.at(0);
        QString longitude = latLong.at(1);

        // Xây dựng điều kiện WHERE cho truy vấn SQL
        QString sql = "SELECT id, timestamp, latitude, longitude, temperature, humidity, pressure, co2, no2, o3 FROM data";
        sql += " WHERE latitude = '" + latitude + "' AND longitude = '" + longitude + "'";

        // Tạo lại bảng với dữ liệu đã lọc
        createTableFromDatabaseWithSQL(db, tableWidget, sql);
    }
}

bool DataHistory::createTableFromDatabaseWithSQL(sqlite3* db, QTableWidget* tableWidget, const QString& sqlQuery)
{
    const char*   sql = sqlQuery.toUtf8().data();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "Failed to fetch data from database:" << sqlite3_errmsg(db);
        return false;
    }

    // Xóa dữ liệu hiện tại trong QTableWidget
    tableWidget->setRowCount(0);

    pthread_mutex_lock(&db_mutex);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        // Retrieve data from the query
        int         id          = sqlite3_column_int(stmt, 0);
        long long   timestamp   = sqlite3_column_int64(stmt, 1);
        const char* latitude    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* longitude   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        const char* temperature = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        const char* humidity    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        const char* pressure    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        const char* co2         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        const char* no2         = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        const char* o3          = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));

        // Thêm dữ liệu vào các cột trong bảng
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(timestamp).toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(temperature));
        tableWidget->setItem(row, 3, new QTableWidgetItem(humidity));
        tableWidget->setItem(row, 4, new QTableWidgetItem(pressure));
        tableWidget->setItem(row, 5, new QTableWidgetItem(co2));
        tableWidget->setItem(row, 6, new QTableWidgetItem(no2));
        tableWidget->setItem(row, 7, new QTableWidgetItem(o3));
    }

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return true;
}
