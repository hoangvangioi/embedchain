#include "about.h"

About::About(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout(this);

    titleLabel = new QLabel("Ứng dụng IoT Thời Tiết", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    layout->addWidget(titleLabel);

    logoLabel = new QLabel(this);
    QPixmap logo(":/icon.png");
    logoLabel->setPixmap(logo.scaled(100, 100, Qt::KeepAspectRatio));
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    descriptionLabel = new QLabel("Ứng dụng này giúp bạn theo dõi và quản lý dữ liệu từ cảm biến IoT về thời tiết, nhiệt độ, độ ẩm, và các yếu tố khác.", this);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("font-size: 16px; color: #666;");
    layout->addWidget(descriptionLabel);

    backButton = new QPushButton("Quay lại", this);
    connect(backButton, &QPushButton::clicked, this, &About::onBackButtonClicked);
    backButton->setStyleSheet("padding: 10px; font-size: 16px; background-color: #4CAF50; color: white; border: none;");
    layout->addWidget(backButton);

    setLayout(layout);
}
About::~About() {}

void About::onBackButtonClicked()
{
    this->close();
}