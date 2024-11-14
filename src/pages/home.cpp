#include "home.h"

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    setObjectName("home_page");
    setWindowTitle("Home");

    layout = new QVBoxLayout(this);
    layout->setObjectName("mainLayout");

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");

    gridLayout = new QGridLayout(contentWidget);
    gridLayout->setObjectName("gridLayout");
    gridLayout->setSpacing(10);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    int row = 0, col = 0;

    // Resize các mảng chứa labels
    labels_4.resize(numFrames);
    labels_5.resize(numFrames);
    labels_23.resize(numFrames);
    location.resize(numFrames);

    for (int i = 0; i < numFrames; ++i)
    {
        frame = new QFrame(contentWidget);
        frame->setObjectName(QString("frame_%1").arg(i));
        frame->setFont(QFont("Roboto", 12, 500, 0));
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        frame->setFixedHeight(350);

        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        frame_17 = new QFrame(frame);
        frame_17->setObjectName(QString("frame_17_%1").arg(i));
        horizontalLayout = new QHBoxLayout(frame_17);
        horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(i));

        widget_16 = new QLabel(frame_17);
        widget_16->setObjectName(QString("widget_16_%1").arg(i));
        widget_16->setFixedSize(32, 32);
        widget_16->setPixmap(QIcon(":/svg/location.svg").pixmap(30, 30));

        horizontalLayout->addWidget(widget_16);

        // Gán label cho frame
        location[i] = new QLabel(frame_17);
        location[i]->setObjectName(QString("location_%1").arg(i));
        location[i]->setAlignment(Qt::AlignVCenter);
        horizontalLayout->addWidget(location[i]);

        // Thêm button và các widget khác
        pushButton_2 = new QPushButton(frame_17);
        pushButton_2->setObjectName(QString("pushButton_2_%1").arg(i));
        pushButton_2->setText("MORE");
        pushButton_2->setFixedSize(100, 20);
        horizontalLayout->addWidget(pushButton_2);

        verticalLayout->addWidget(frame_17);

        // Thêm widget cho các thông tin khác (nhiệt độ, độ ẩm...)
        widget_8 = new QWidget(frame);
        widget_8->setObjectName(QString("widget_8_%1").arg(i));
        horizontalLayout_6 = new QHBoxLayout(widget_8);
        horizontalLayout_6->setObjectName(QString("horizontalLayout_6_%1").arg(i));

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString("horizontalLayout_5_%1").arg(i));
        widget = new QLabel(widget_8);
        widget->setObjectName(QString("widget_%1").arg(i));
        widget->setPixmap(QIcon(":/svg/sunny.svg").pixmap(width() / 3, height() / 3));
        widget->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(widget);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString("verticalLayout_7_%1").arg(i));

        label_6 = new QLabel(widget_8);
        label_6->setObjectName(QString("label_6_%1").arg(i));
        label_6->setText("Temperature");
        label_6->setMaximumHeight(30);
        verticalLayout_7->addWidget(label_6);

        labels_4[i] = new QLabel(widget_8); // Cập nhật từng label
        labels_4[i]->setObjectName(QString("label_4_%1").arg(i));
        labels_4[i]->setFont(QFont("Roboto", 36, 700, false));
        labels_4[i]->setAlignment(Qt::AlignVCenter);
        verticalLayout_7->addWidget(labels_4[i]);

        horizontalLayout_5->addLayout(verticalLayout_7);
        horizontalLayout_6->addLayout(horizontalLayout_5);
        verticalLayout->addWidget(widget_8);

        frame_6 = new QFrame(frame);
        frame_6->setObjectName(QString("frame_6_%1").arg(i));
        frame_6->setFrameShape(QFrame::Shape::StyledPanel);
        frame_6->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_6);
        horizontalLayout_4->setObjectName(QString("horizontalLayout_4_%1").arg(i));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString("horizontalLayout_3_%1").arg(i));

        widget_2 = new QLabel(frame_6);
        widget_2->setObjectName(QString("widget_2_%1").arg(i));
        widget_2->setPixmap(QIcon(":/svg/humidity.svg").pixmap(50, 50));

        horizontalLayout_3->addWidget(widget_2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString("verticalLayout_6_%1").arg(i));

        // Đảm bảo các label khác cũng được thiết lập tương tự
        label_9 = new QLabel(frame_6);
        label_9->setObjectName(QString("label_9_%1").arg(i));
        label_9->setText("Humidity");
        verticalLayout_6->addWidget(label_9);

        labels_5[i] = new QLabel(frame_6);
        labels_5[i]->setObjectName(QString("label_5_%1").arg(i));
        verticalLayout_6->addWidget(labels_5[i]);

        horizontalLayout_3->addLayout(verticalLayout_6);

        horizontalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString("horizontalLayout_2_%1").arg(i));
        widget_3 = new QLabel(frame_6);
        widget_3->setObjectName(QString("widget_3_%1").arg(i));
        widget_3->setPixmap(QIcon(":/svg/pressure.svg").pixmap(50, 50));

        horizontalLayout_2->addWidget(widget_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString("verticalLayout_5_%1").arg(i));

        label_13 = new QLabel(frame_17);
        label_13->setObjectName(QString("label_13_%1").arg(i));
        label_13->setText("Pressure");
        verticalLayout_5->addWidget(label_13);

        labels_23[i] = new QLabel(frame_17);
        labels_23[i]->setObjectName(QString("label_23_%1").arg(i));
        verticalLayout_5->addWidget(labels_23[i]);

        horizontalLayout_2->addLayout(verticalLayout_5);

        horizontalLayout_4->addLayout(horizontalLayout_2);

        verticalLayout->addWidget(frame_6);

        // Cấu hình stretch factor để chia đều không gian
        verticalLayout->setStretchFactor(frame_17, 1);
        verticalLayout->setStretchFactor(widget_8, 4);
        verticalLayout->setStretchFactor(frame_6, 2);

        // Đặt vào trong gridLayout
        gridLayout->addWidget(frame, row, col, 1, 1);

        // Cập nhật row và col để thêm frame vào các vị trí tiếp theo trong grid
        col++;
        if (col > 1)
        {
            col = 0;
            row++;
        }
    }

    scrollArea->setWidget(contentWidget);
    layout->addWidget(scrollArea);

    // Đặt timer để cập nhật dữ liệu từ cơ sở dữ liệu mỗi giây
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HomePage::updateSensorDataFromDb);
    timer->start(1000);
}

HomePage::~HomePage() {}

void HomePage::queryLatestDataForCoordinates()
{
    pthread_mutex_lock(&db_mutex);

    sqlite3_stmt *stmt;
    const char   *sql = "SELECT latitude, longitude FROM data GROUP BY latitude, longitude";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        return;
    }

    sensorData.clear();

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *latitude  = (const char *) sqlite3_column_text(stmt, 0);
        const char *longitude = (const char *) sqlite3_column_text(stmt, 1);

        // Truy vấn dữ liệu cho từng tọa độ
        sqlite3_stmt *stmt_data;
        const char   *sql_data =
            "SELECT temperature, humidity, pressure, co2, no2, o3 "
            "FROM data WHERE latitude = ? AND longitude = ? "
            "ORDER BY timestamp DESC LIMIT 1";

        if (sqlite3_prepare_v2(db, sql_data, -1, &stmt_data, 0) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing data query: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt_data);
            continue;
        }

        sqlite3_bind_text(stmt_data, 1, latitude, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt_data, 2, longitude, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt_data) == SQLITE_ROW)
        {
            SensorData data;
            strncpy(data.latitude, latitude, sizeof(data.latitude));
            strncpy(data.longitude, longitude, sizeof(data.longitude));
            data.temperature = atof((const char *) sqlite3_column_text(stmt_data, 0));
            data.humidity    = atof((const char *) sqlite3_column_text(stmt_data, 1));
            data.pressure    = atof((const char *) sqlite3_column_text(stmt_data, 2));
            data.co2         = atof((const char *) sqlite3_column_text(stmt_data, 3));
            data.no2         = atof((const char *) sqlite3_column_text(stmt_data, 4));
            data.o3          = atof((const char *) sqlite3_column_text(stmt_data, 5));

            sensorData.append(data);
        }

        sqlite3_finalize(stmt_data);
    }

    sqlite3_finalize(stmt);

    pthread_mutex_unlock(&db_mutex);
}

void HomePage::updateLabels()
{
    // Đảm bảo rằng không có quá nhiều dữ liệu so với số lượng widgets
    int numData = sensorData.size();
    for (int i = 0; i < numFrames; ++i)
    {
        if (i < numData)
        {
            // Cập nhật dữ liệu cho các labels tương ứng với tọa độ
            location[i]->setText(QString("Longitude: %1 - Latitude: %2").arg(sensorData[i].longitude).arg(sensorData[i].latitude));
            labels_4[i]->setText(QString::number(sensorData[i].temperature) + "°C");
            labels_5[i]->setText(QString::number(sensorData[i].humidity) + "%");
            labels_23[i]->setText(QString::number(sensorData[i].pressure) + " hPa");
        }
    }
}

void HomePage::updateSensorDataFromDb()
{
    queryLatestDataForCoordinates();
    updateLabels();
}
