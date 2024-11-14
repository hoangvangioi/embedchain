#include "mainwindow.h"

#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    Settings::instance();
    Theme::instance();
    this->setStyle(QStyleFactory::create("Fusion"));
    this->setFont(QFont("Roboto", 12));
    this->setWindowTitle(Settings::instance().getAppName());
    this->setWindowRole("About");
    this->resize(Settings::instance().getStartupSize());
    this->setMinimumSize(Settings::instance().getMinimumSize());

    setupUI();
    initializeMenus();

    QPalette palette = this->palette();
    Theme::instance().applyTheme(palette);
    this->setPalette(palette);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // Add central widget to app
    centralWidget = new QWidget(this);
    centralWidget->setFont(QFont(Settings::instance().getFontFamily(), Settings::instance().getFontTextSize()));

    centralWidgetLayout = new QVBoxLayout(centralWidget);
    centralWidgetLayout->setContentsMargins(0, 0, 0, 0);

    windowl            = new QFrame(this);
    QHBoxLayout *h_l_o = new QHBoxLayout(windowl);
    h_l_o->setObjectName("pod_bg_app");
    h_l_o->setContentsMargins(8, 8, 8, 8);
    h_l_o->setSpacing(2);
    centralWidgetLayout->addWidget(windowl);

    setupLeftMenu();
    setupRightLayout();
    setCentralWidget(centralWidget);
}

void MainWindow::setupLeftMenu()
{
    leftMenuFrame = new QFrame(this);
    leftMenuFrame->setMaximumSize(Settings::instance().getLeftMenuContentMargins() * 2 + Settings::instance().getLeftMenuSize().width(), 17280);
    leftMenuFrame->setMinimumSize(Settings::instance().getLeftMenuContentMargins() * 2 + Settings::instance().getLeftMenuSize().width(), 0);

    QHBoxLayout *leftMenuLayout = new QHBoxLayout(leftMenuFrame);
    leftMenuLayout->setContentsMargins(Settings::instance().getLeftMenuContentMargins(),
                                       Settings::instance().getLeftMenuContentMargins(),
                                       Settings::instance().getLeftMenuContentMargins(),
                                       Settings::instance().getLeftMenuContentMargins());

    leftMenu = new LeftMenu(leftMenuFrame, centralWidget);
    leftMenuLayout->addWidget(leftMenu);
    windowl->layout()->addWidget(leftMenuFrame);
}

void MainWindow::setupRightLayout()
{
    rightAppFrame  = new QFrame(this);
    rightAppLayout = new QVBoxLayout(rightAppFrame);
    rightAppLayout->setContentsMargins(3, 3, 3, 3);
    rightAppLayout->setSpacing(6);

    titleBarFrame = new QFrame(this);
    titleBarFrame->setFixedHeight(42);
    QVBoxLayout *titleBarLayout = new QVBoxLayout(titleBarFrame);
    titleBarLayout->setContentsMargins(5, 0, 5, 0);

    // Custom Title Bar
    titleBar = new TitleBar(this, centralWidget);
    titleBarLayout->addWidget(titleBar);
    rightAppLayout->addWidget(titleBarFrame);

    contentAreaFrame               = new QFrame(this);
    QHBoxLayout *contentAreaLayout = new QHBoxLayout(contentAreaFrame);
    contentAreaLayout->setContentsMargins(0, 0, 0, 0);
    contentAreaLayout->setSpacing(0);

    QFrame *contentAreaLeftFrame = new QFrame(this);
    loadPages                    = new MainPages();
    loadPages->setupUi(contentAreaLeftFrame);

    rightColumnFrame = new QFrame(this);
    rightColumnFrame->setFixedWidth(Settings::instance().getRightColumnSize().width());
    contentAreaRightLayout = new QVBoxLayout(rightColumnFrame);
    contentAreaRightLayout->setContentsMargins(5, 5, 5, 5);
    contentAreaRightLayout->setSpacing(0);

    // Right BG
    contentAreaRightBgFrame = new QFrame();
    contentAreaRightBgFrame->setObjectName("content_area_right_bg_frame");
    contentAreaRightBgFrame->setStyleSheet(QString("#content_area_right_bg_frame { border-radius: 8px; background-color: %1; }").arg(Theme::instance().getBgTwoColor().name()));

    contentAreaRightLayout->addWidget(contentAreaRightBgFrame);
    contentAreaLayout->addWidget(contentAreaLeftFrame);
    rightAppLayout->addWidget(titleBarFrame);
    rightAppLayout->addWidget(contentAreaFrame);

    windowl->layout()->addWidget(rightAppFrame);
}

void MainWindow::setPage(QWidget *page)
{
    loadPages->setCurrentPage(page);
}

void MainWindow::initializeMenus()
{
    // Khởi tạo danh sách menu bên trái
    add_left_menus = {
        {{"btn_icon", "home.svg"}, {"btn_id", "btn_home"}, {"btn_text", "Home"}, {"btn_tooltip", "Home page"}, {"show_top", true}, {"is_active", true}},
        {{"btn_icon", "chart.svg"}, {"btn_id", "btn_widgets"}, {"btn_text", "Show Widgets"}, {"btn_tooltip", "Show widgets"}, {"show_top", true}, {"is_active", false}},
        {{"btn_icon", "table.svg"}, {"btn_id", "btn_add_user"}, {"btn_text", "Add Users"}, {"btn_tooltip", "Add users"}, {"show_top", true}, {"is_active", false}},
        {{"btn_icon", "table.svg"}, {"btn_id", "btn_new_file"}, {"btn_text", "New File"}, {"btn_tooltip", "Create new file"}, {"show_top", true}, {"is_active", false}},
        {{"btn_icon", "map.svg"}, {"btn_id", "btn_save"}, {"btn_text", "Save File"}, {"btn_tooltip", "Save file"}, {"show_top", true}, {"is_active", false}},
        {{"btn_icon", "settings.svg"}, {"btn_id", "btn_settings"}, {"btn_text", "Settings"}, {"btn_tooltip", "Open settings"}, {"is_active", false}},
        {{"btn_icon", "info.svg"}, {"btn_id", "btn_info"}, {"btn_text", "Information"}, {"btn_tooltip", "Open informations"}, {"is_active", false}}};

    // Add Menus to left menu
    leftMenu->addMenus(add_left_menus);
    connect(leftMenu, &LeftMenu::clicked, this, &MainWindow::btnClicked);
    connect(leftMenu, &LeftMenu::released, this, &MainWindow::btnReleased);

    // Title Bar Menus
    connect(titleBar, &TitleBar::clicked, this, &MainWindow::toggleTheme);

    // Set Title and Initial Page
    titleBar->setTitle(Settings::instance().getAppName());
    setPage(loadPages->home_page);
}

void MainWindow::btnClicked(QWidget *sender)
{
    auto *button = qobject_cast<QPushButton *>(sender);
    if (!button) return;

    QString btn = button->objectName();

    QMap<QString, std::function<void()>> buttonActions = {
        {"btn_home",
         [this]()
         {
             setPage(loadPages->home_page);
             titleBar->setTitle("Home Page");
         }},
        {"btn_widgets",
         [this]()
         {
             setPage(loadPages->page_analytics);
             titleBar->setTitle("Analytics");
         }},
        {"btn_add_user",
         [this]()
         {
             setPage(loadPages->page_blockchain);
             titleBar->setTitle("Blockchain History");
         }},
        {"btn_new_file",
         [this]()
         {
             setPage(loadPages->page_data);
             titleBar->setTitle("Data History");
         }},
        {"btn_save",
         [this]()
         {
             setPage(loadPages->map_page);
             titleBar->setTitle("Map Pages");
         }},
        {"btn_info",
         [this]()
         {
             About *about = new About(this);
             titleBar->setTitle("About");
             about->exec();
         }},
        {"btn_settings",
         [this]()
         {
             setPage(loadPages->page_preferences);
             titleBar->setTitle("Settings");
         }},
    };

    if (buttonActions.contains(btn))
    {
        leftMenu->selectOnlyOne(btn);
        buttonActions[btn]();
    }

    qDebug() << "Button" << btn << "clicked!";
}

void MainWindow::btnReleased(QWidget *sender)
{
    QPushButton *button = qobject_cast<QPushButton *>(sender);

    if (button)
    {
        qDebug() << "Button" << button->objectName() << "released!";
    }
}

void MainWindow::toggleTheme()
{
    Theme::instance().toggleTheme();
    QPalette palette = this->palette();
    Theme::instance().applyTheme(palette);
    setPalette(palette);

    // contentAreaRightBgFrame->setStyleSheet(QString("#content_area_right_bg_frame { border-radius: 8px; background-color: %1; }").arg(Theme::instance().getBgTwoColor().name()));
}
