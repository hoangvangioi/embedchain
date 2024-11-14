#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QFontDatabase>
#include <QObject>
#include <QResource>
#include <QSettings>
#include <QSize>
#include <QString>

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings &instance()
    {
        static Settings instance; // Tạo một thể hiện duy nhất
        return instance;
    }

    // Getters for Application Settings
    QString getAppName() const;
    QString getVersion() const;
    QString getCopyright() const;
    int     getYear() const;
    QString getThemeName() const;
    bool    isCustomTitleBar() const;
    QSize   getStartupSize() const;
    QSize   getMinimumSize() const;
    QSize   getLeftMenuSize() const;
    int     getLeftMenuContentMargins() const;
    QSize   getLeftColumnSize() const;
    QSize   getRightColumnSize() const;
    int     getTimeAnimation() const;
    QString getFontFamily() const;
    int     getFontTitleSize() const;
    int     getFontTextSize() const;

private:
    explicit Settings(QObject *parent = nullptr);
    Settings(const Settings &)       = delete; // Ngăn không cho sao chép
    void operator=(const Settings &) = delete; // Ngăn không cho gán

    QSettings _settings;
};

#endif // SETTINGS_H
