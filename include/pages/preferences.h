#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

class Preferences : public QWidget
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void saveSettings();

private:
    QComboBox   *themeComboBox;
    QComboBox   *languageComboBox;
    QCheckBox   *notificationsCheckBox;
    QPushButton *saveButton;
};

#endif // PREFERENCES_H
