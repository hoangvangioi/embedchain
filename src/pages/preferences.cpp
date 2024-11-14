#include "preferences.h"

#include <QFontDatabase>
#include <QIcon>
#include <QMessageBox>
#include <QSettings>

Preferences::Preferences(QWidget *parent) : QWidget(parent)
{
    // Thiết lập kiểu dáng của cửa sổ
    this->setWindowTitle("Cài Đặt");
    this->setMinimumSize(500, 500);

    // Tạo layout chính
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignVCenter);
    layout->setContentsMargins(height() / 4, 0, height() / 4, 0);
    layout->setSpacing(20);

    QLabel *label = new QLabel("Comming Soon", this);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont("Roboto", 24, 700));
    layout->addWidget(label);

    // Tạo Label và ComboBox cho chủ đề
    QLabel *themeLabel = new QLabel("Chọn Chủ Đề", this);
    themeLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(themeLabel);

    themeComboBox = new QComboBox(this);
    themeComboBox->addItem("Light");
    themeComboBox->addItem("Dark");
    themeComboBox->setStyleSheet("font-size: 14px; padding: 5px;");
    layout->addWidget(themeComboBox);

    // Tạo Label và ComboBox cho ngôn ngữ
    QLabel *languageLabel = new QLabel("Chọn Ngôn Ngữ", this);
    languageLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(languageLabel);

    languageComboBox = new QComboBox(this);
    languageComboBox->addItem("English");
    languageComboBox->addItem("Vietnamese");
    languageComboBox->setStyleSheet("font-size: 14px; padding: 5px;");
    layout->addWidget(languageComboBox);

    // Tạo Checkbox cho thông báo
    notificationsCheckBox = new QCheckBox("Bật Thông Báo", this);
    notificationsCheckBox->setStyleSheet("font-size: 14px;");
    layout->addWidget(notificationsCheckBox);

    // Tạo Button Lưu Cài Đặt với hiệu ứng hover
    saveButton = new QPushButton("Lưu Cài Đặt", this);
    saveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50; color: white; font-size: 14px; padding: 10px; border-radius: 5px; border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}");
    layout->addWidget(saveButton);

    // Kết nối nút lưu với slot saveSettings
    connect(saveButton, &QPushButton::clicked, this, &Preferences::saveSettings);

    // Đọc và thiết lập các giá trị mặc định từ QSettings (nếu có)
    QSettings settings;
    themeComboBox->setCurrentText(settings.value("theme", "Light").toString());
    languageComboBox->setCurrentText(settings.value("language", "English").toString());
    notificationsCheckBox->setChecked(settings.value("notifications", false).toBool());
}

Preferences::~Preferences() {}

void Preferences::saveSettings()
{
    // Lưu cài đặt vào QSettings
    QSettings settings;
    settings.setValue("theme", themeComboBox->currentText());
    settings.setValue("language", languageComboBox->currentText());
    settings.setValue("notifications", notificationsCheckBox->isChecked());

    // Hiển thị thông báo khi lưu cài đặt thành công
    QMessageBox::information(this, "Cài Đặt", "Cài đặt đã được lưu.");
}