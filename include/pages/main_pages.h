#ifndef MAINPAGES_H
#define MAINPAGES_H

#include "about.h"
#include "analytics.h"
#include "blockchain_history.h"
#include "data_history.h"
#include "home.h"
#include "location_map.h"
#include "preferences.h"

#include <QCoreApplication>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class MainPages
{
public:
    void setupUi(QWidget *parent)
    {
        if (!parent->objectName().isEmpty()) parent->setObjectName("MainPages");
        parent->resize(860, 600);

        main_pages_layout = new QVBoxLayout(parent);
        main_pages_layout->setSpacing(0);
        main_pages_layout->setContentsMargins(5, 5, 5, 5);

        pages = new QStackedWidget(parent);
        pages->setObjectName("pages");

        // Add each page
        home_page = new HomePage();
        pages->addWidget(home_page);
        page_analytics = new Analytics();
        pages->addWidget(page_analytics);
        page_blockchain = new BlockchainHistory();
        pages->addWidget(page_blockchain);
        page_data = new DataHistory();
        pages->addWidget(page_data);
        map_page = new LocationMap();
        pages->addWidget(map_page);
        page_preferences = new Preferences();
        pages->addWidget(page_preferences);

        main_pages_layout->addWidget(pages);

        pages->setCurrentIndex(0);
    }

    void setCurrentPage(QWidget *page) { pages->setCurrentWidget(page); }

    HomePage          *home_page;
    Analytics         *page_analytics;
    BlockchainHistory *page_blockchain;
    DataHistory       *page_data;
    LocationMap       *map_page;
    Preferences       *page_preferences;

private:
    QVBoxLayout    *main_pages_layout;
    QStackedWidget *pages;
};

#endif // MAINPAGES_H
