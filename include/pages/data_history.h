#ifndef DATA_HISTORY_H
#define DATA_HISTORY_H

#include "database.h"
#include "sqlite3.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class DataHistory : public QWidget
{
    Q_OBJECT

public:
    explicit DataHistory(QWidget *parent = nullptr);
    ~DataHistory();

private slots:
    void onReloadButtonClicked();
    void onFilterButtonClicked();

private:
    sqlite3      *db = global_db;
    QVBoxLayout  *layout;
    QTableWidget *tableWidget;
    QPushButton  *reloadButton;

    QPushButton *filterButton;
    QComboBox   *locationFilter;

    bool createTableFromDatabase(sqlite3 *db, QTableWidget *tableWidget);
    bool createTableFromDatabaseWithSQL(sqlite3 *db, QTableWidget *tableWidget, const QString &sqlQuery);
};

#endif // DATA_HISTORY_H
