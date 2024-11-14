#include "settings.h"

// Constructor
Settings::Settings(QObject *parent) : QObject(parent), _settings(":/settings.ini", QSettings::IniFormat)
{
    QResource::registerResource(":/settings.ini");

    // Tải các font vào ứng dụng từ file
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Thin.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-ThinItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Black.ttf");
}

// Getters for Application Settings
QString Settings::getAppName() const
{
    return _settings.value("Application/app_name", "DefaultApp").toString();
}

QString Settings::getVersion() const
{
    return _settings.value("Application/version", "v1.0.0").toString();
}

QString Settings::getCopyright() const
{
    return _settings.value("Application/copyright", "DefaultCopyright").toString();
}

int Settings::getYear() const
{
    return _settings.value("Application/year", 2021).toInt();
}

QString Settings::getThemeName() const
{
    return _settings.value("Application/theme_name", "default_theme").toString();
}

bool Settings::isCustomTitleBar() const
{
    return _settings.value("Application/custom_title_bar", true).toBool();
}

QSize Settings::getStartupSize() const
{
    int w = _settings.value("Window/startup_width", 1260).toInt();
    int h = _settings.value("Window/startup_height", 720).toInt();
    return QSize(w, h);
}

QSize Settings::getMinimumSize() const
{
    int w = _settings.value("Window/minimum_width", 960).toInt();
    int h = _settings.value("Window/minimum_height", 540).toInt();
    return QSize(w, h);
}

QSize Settings::getLeftMenuSize() const
{
    int minimum = _settings.value("Menu/left_menu_minimum", 50).toInt();
    int maximum = _settings.value("Menu/left_menu_maximum", 240).toInt();
    return QSize(minimum, maximum);
}

int Settings::getLeftMenuContentMargins() const
{
    return _settings.value("Menu/left_menu_content_margins", 3).toInt();
}

QSize Settings::getLeftColumnSize() const
{
    int minimum = _settings.value("LeftColumn/left_column_minimum", 0).toInt();
    int maximum = _settings.value("LeftColumn/left_column_maximum", 240).toInt();
    return QSize(minimum, maximum);
}

QSize Settings::getRightColumnSize() const
{
    int minimum = _settings.value("RightColumn/right_column_minimum", 0).toInt();
    int maximum = _settings.value("RightColumn/right_column_maximum", 240).toInt();
    return QSize(minimum, maximum);
}

int Settings::getTimeAnimation() const
{
    return _settings.value("Animation/time_animation", 500).toInt();
}

QString Settings::getFontFamily() const
{
    return _settings.value("Font/font_family", "Roboto").toString();
}

int Settings::getFontTitleSize() const
{
    return _settings.value("Font/font_title_size", 10).toInt();
}

int Settings::getFontTextSize() const
{
    return _settings.value("Font/font_text_size", 9).toInt();
}
