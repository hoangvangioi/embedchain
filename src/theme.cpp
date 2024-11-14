#include "theme.h"

#include <QPalette>

Theme::Theme() : _themes(":/themes.ini", QSettings::IniFormat), currentTheme("dark")
{
    QResource::registerResource(":/themes.ini");
}

void Theme::applyTheme(QPalette &palette)
{
    palette.setColor(QPalette::Window, QColor(_themes.value(currentTheme + "/Window").toString()));
    palette.setColor(QPalette::WindowText, QColor(_themes.value(currentTheme + "/WindowText").toString()));
    palette.setColor(QPalette::Base, QColor(_themes.value(currentTheme + "/Base").toString()));
    palette.setColor(QPalette::AlternateBase, QColor(_themes.value(currentTheme + "/AlternateBase").toString()));
    palette.setColor(QPalette::Text, QColor(_themes.value(currentTheme + "/Text").toString()));
    palette.setColor(QPalette::Button, QColor(_themes.value(currentTheme + "/Button").toString()));
    palette.setColor(QPalette::ButtonText, QColor(_themes.value(currentTheme + "/ButtonText").toString()));
    palette.setColor(QPalette::Link, QColor(_themes.value(currentTheme + "/Link").toString()));
    palette.setColor(QPalette::LinkVisited, QColor(_themes.value(currentTheme + "/LinkVisited").toString()));
    palette.setColor(QPalette::Highlight, QColor(_themes.value(currentTheme + "/Highlight").toString()));
    palette.setColor(QPalette::HighlightedText, QColor(_themes.value(currentTheme + "/HighlightedText").toString()));
    palette.setColor(QPalette::BrightText, QColor(_themes.value(currentTheme + "/BrightText").toString()));
    palette.setColor(QPalette::Dark, QColor(_themes.value(currentTheme + "/Dark").toString()));
    palette.setColor(QPalette::Light, QColor(_themes.value(currentTheme + "/Light").toString()));
    palette.setColor(QPalette::Midlight, QColor(_themes.value(currentTheme + "/Midlight").toString()));
    palette.setColor(QPalette::Mid, QColor(_themes.value(currentTheme + "/Mid").toString()));
    palette.setColor(QPalette::Shadow, QColor(_themes.value(currentTheme + "/Shadow").toString()));
}

void Theme::toggleTheme()
{
    currentTheme = (currentTheme == "light") ? "dark" : "light";
}

QString Theme::getCurrentTheme()
{
    return currentTheme;
}

// Getters for Theme
QColor Theme::getDarkOneColor() const
{
    return QColor(_themes.value(currentTheme + "/dark_one", "#282a36").toString());
}

QColor Theme::getDarkTwoColor() const
{
    return QColor(_themes.value(currentTheme + "/dark_two", "#2B2E3B").toString());
}

QColor Theme::getDarkThreeColor() const
{
    return QColor(_themes.value(currentTheme + "/dark_three", "#333645").toString());
}

QColor Theme::getDarkFourColor() const
{
    return QColor(_themes.value(currentTheme + "/dark_four", "#3C4052").toString());
}

QColor Theme::getBgOneColor() const
{
    return QColor(_themes.value(currentTheme + "/bg_one", "#44475a").toString());
}

QColor Theme::getBgTwoColor() const
{
    return QColor(_themes.value(currentTheme + "/bg_two", "#4D5066").toString());
}

QColor Theme::getBgThreeColor() const
{
    return QColor(_themes.value(currentTheme + "/bg_three", "#595D75").toString());
}

QColor Theme::getIconColor() const
{
    return QColor(_themes.value(currentTheme + "/icon_color", "#c3ccdf").toString());
}

QColor Theme::getIconHoverColor() const
{
    return QColor(_themes.value(currentTheme + "/icon_hover", "#dce1ec").toString());
}

QColor Theme::getIconPressedColor() const
{
    return QColor(_themes.value(currentTheme + "/icon_pressed", "#ff79c6").toString());
}

QColor Theme::getIconActiveColor() const
{
    return QColor(_themes.value(currentTheme + "/icon_active", "#f5f6f9").toString());
}

QColor Theme::getContextColor() const
{
    return QColor(_themes.value(currentTheme + "/context_color", "#ff79c6").toString());
}

QColor Theme::getContextHoverColor() const
{
    return QColor(_themes.value(currentTheme + "/context_hover", "#FF84D7").toString());
}

QColor Theme::getContextPressedColor() const
{
    return QColor(_themes.value(currentTheme + "/context_pressed", "#FF90DD").toString());
}

QColor Theme::getTextTitleColor() const
{
    return QColor(_themes.value(currentTheme + "/text_title", "#dce1ec").toString());
}

QColor Theme::getTextForegroundColor() const
{
    return QColor(_themes.value(currentTheme + "/text_foreground", "#f8f8f2").toString());
}

QColor Theme::getTextDescriptionColor() const
{
    return QColor(_themes.value(currentTheme + "/text_description", "#979EC7").toString());
}

QColor Theme::getTextActiveColor() const
{
    return QColor(_themes.value(currentTheme + "/text_active", "#dce1ec").toString());
}

QColor Theme::getWhiteColor() const
{
    return QColor(_themes.value(currentTheme + "/white", "#f5f6f9").toString());
}

QColor Theme::getPinkColor() const
{
    return QColor(_themes.value(currentTheme + "/pink", "#ff79c6").toString());
}

QColor Theme::getGreenColor() const
{
    return QColor(_themes.value(currentTheme + "/green", "#00ff7f").toString());
}

QColor Theme::getRedColor() const
{
    return QColor(_themes.value(currentTheme + "/red", "#ff5555").toString());
}

QColor Theme::getYellowColor() const
{
    return QColor(_themes.value(currentTheme + "/yellow", "#f1fa8c").toString());
}

Color Theme::colorParams() const
{
    Color params;
    params.dark_one        = getDarkOneColor();
    params.dark_two        = getDarkTwoColor();
    params.dark_three      = getDarkThreeColor();
    params.bg_one          = getBgOneColor();
    params.bg_two          = getBgTwoColor();
    params.bg_three        = getBgThreeColor();
    params.icon_color      = getIconColor();
    params.icon_hover      = getIconHoverColor();
    params.icon_pressed    = getIconPressedColor();
    params.icon_active     = getIconActiveColor();
    params.context_color   = getContextColor();
    params.text_foreground = getTextForegroundColor();
    params.text_active     = getTextActiveColor();
    return params;
}
