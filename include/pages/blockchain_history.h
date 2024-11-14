#ifndef BLOCKCHAIN_HISTORY_H
#define BLOCKCHAIN_HISTORY_H

#include "database.h"
#include "sqlite3.h"

#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class BlockchainHistory : public QWidget
{
    Q_OBJECT

public:
    explicit BlockchainHistory(QWidget *parent = nullptr);
    ~BlockchainHistory();

private slots:
    void onReloadButtonClicked();

private:
    sqlite3 *db = global_db;

    QVBoxLayout  *layout;
    QTableWidget *tableWidget;
    QPushButton  *reloadButton;

    bool createTableFromDatabase(sqlite3 *db, QTableWidget *tableWidget);
};

#endif // BLOCKCHAIN_HISTORY_H
