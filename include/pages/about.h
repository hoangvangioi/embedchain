#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();

private slots:
    void onBackButtonClicked();

private:
    QLabel      *titleLabel;
    QLabel      *descriptionLabel;
    QLabel      *logoLabel;
    QPushButton *backButton;
    QVBoxLayout *layout;
};

#endif // ABOUT_H
