#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "leftmenu.h"
#include "main_pages.h"
#include "settings.h"
#include "sqlite3.h"
#include "theme.h"
#include "titlebar.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMouseEvent>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QString>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void btnClicked(QWidget *sender);
    void btnReleased(QWidget *sender);
    void toggleTheme();

private:
    // UI elements
    QWidget     *centralWidget;
    QVBoxLayout *centralWidgetLayout;
    QFrame      *leftMenuFrame;
    QFrame      *rightAppFrame;
    QVBoxLayout *rightAppLayout;
    QVBoxLayout *contentAreaRightLayout;
    QFrame      *titleBarFrame;
    QFrame      *contentAreaFrame;
    QFrame      *rightColumnFrame;
    QFrame      *contentAreaRightBgFrame;
    MainPages   *loadPages;

    QList<QMap<QString, QVariant>> add_left_menus;
    QMap<QString, QVariant>        add_swtheme;

    TitleBar *titleBar;
    QFrame   *windowl;
    LeftMenu *leftMenu;

    // Methods
    void setupUI();
    void setupLeftMenu();
    void setupRightLayout();
    void setPage(QWidget *page);
    void initializeMenus();
};

#endif // MAINWINDOW_H
