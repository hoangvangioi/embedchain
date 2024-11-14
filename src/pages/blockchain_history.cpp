#include "blockchain_history.h"

#include <QDateTime>
#include <QDebug>
#include <QHeaderView>

BlockchainHistory::BlockchainHistory(QWidget* parent) : QWidget(parent), tableWidget(new QTableWidget(this)), reloadButton(new QPushButton("Reload", this))
{
    this->setObjectName("data_history");
    this->setStyleSheet("font-size: 10pt");

    layout = new QVBoxLayout(this);
    layout->addWidget(reloadButton);
    layout->addWidget(tableWidget);

    // Đảm bảo bảng mở rộng và phủ hết widget ngoài
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidget->setColumnCount(9);

    tableWidget->setHorizontalHeaderLabels({"Block ID", "Block Index", "Previous Hash", "Hash", "Transaction Count", "Transaction ID", "Sender", "Timestamp", "Data Size"});

    connect(reloadButton, &QPushButton::clicked, this, &BlockchainHistory::onReloadButtonClicked);

    createTableFromDatabase(db, tableWidget);
}

BlockchainHistory::~BlockchainHistory() {}

void BlockchainHistory::onReloadButtonClicked()
{
    createTableFromDatabase(db, tableWidget);
}

bool BlockchainHistory::createTableFromDatabase(sqlite3* db, QTableWidget* tableWidget)
{
    const char* sql =
        "SELECT blocks.id, blocks.block_index, blocks.previous_hash, blocks.hash, blocks.transaction_count, "
        "transactions.id AS transaction_id, transactions.sender, transactions.timestamp, transactions.data "
        "FROM blocks "
        "LEFT JOIN transactions ON blocks.block_index = transactions.block_index;";

    sqlite3_stmt* stmt;
    int           rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
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

        // Lấy dữ liệu từ câu truy vấn
        int         id                = sqlite3_column_int(stmt, 0);
        int         block_index       = sqlite3_column_int(stmt, 1);
        const char* previous_hash     = (const char*) sqlite3_column_text(stmt, 2);
        const char* hash              = (const char*) sqlite3_column_text(stmt, 3);
        int         transaction_count = sqlite3_column_int(stmt, 4);
        int         transaction_id    = sqlite3_column_int(stmt, 5);
        const char* sender            = (const char*) sqlite3_column_text(stmt, 6);
        long long   timestamp         = sqlite3_column_int64(stmt, 7);
        const void* data              = sqlite3_column_blob(stmt, 8);
        int         data_size         = sqlite3_column_bytes(stmt, 8);

        // Thêm dữ liệu vào các cột trong bảng
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(block_index)));
        tableWidget->setItem(row, 2, new QTableWidgetItem(previous_hash));
        tableWidget->setItem(row, 3, new QTableWidgetItem(hash));
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(transaction_count)));

        // Thêm thông tin giao dịch nếu có
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(transaction_id)));
        tableWidget->setItem(row, 6, new QTableWidgetItem(sender));
        tableWidget->setItem(row, 7, new QTableWidgetItem(QDateTime::fromMSecsSinceEpoch(timestamp).toString()));
        tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(data_size))); // Dữ liệu (chỉ hiển thị kích thước)
    }

    sqlite3_finalize(stmt);
    pthread_mutex_unlock(&db_mutex);

    return true;
}
