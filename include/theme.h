#pragma once

#include <QColor>
#include <QObject>
#include <QResource>
#include <QSettings>
#include <QString>

typedef struct
{
    QColor dark_one;
    QColor dark_two;
    QColor dark_three;
    QColor bg_one;
    QColor bg_two;
    QColor bg_three;
    QColor icon_color;
    QColor icon_hover;
    QColor icon_pressed;
    QColor icon_active;
    QColor context_color;
    QColor text_foreground;
    QColor text_active;
} Color;

class Theme : public QObject
{
    Q_OBJECT

public:
    static Theme &instance()
    {
        static Theme instance; // Tạo một thể hiện duy nhất
        return instance;
    }

    static Color color()
    {
        static Color _color = instance().colorParams(); // Gán giá trị chỉ một lần
        return _color;
    }

    void    applyTheme(QPalette &palette);
    void    toggleTheme();
    QString getCurrentTheme();

    // Theme color getters
    QColor getDarkOneColor() const;
    QColor getDarkTwoColor() const;
    QColor getDarkThreeColor() const;
    QColor getDarkFourColor() const;
    QColor getBgOneColor() const;
    QColor getBgTwoColor() const;
    QColor getBgThreeColor() const;
    QColor getIconColor() const;
    QColor getIconHoverColor() const;
    QColor getIconPressedColor() const;
    QColor getIconActiveColor() const;
    QColor getContextColor() const;
    QColor getContextHoverColor() const;
    QColor getContextPressedColor() const;
    QColor getTextTitleColor() const;
    QColor getTextForegroundColor() const;
    QColor getTextDescriptionColor() const;
    QColor getTextActiveColor() const;
    QColor getWhiteColor() const;
    QColor getPinkColor() const;
    QColor getGreenColor() const;
    QColor getRedColor() const;
    QColor getYellowColor() const;
    Color  colorParams() const;

private:
    explicit Theme();
    Theme(const Theme &)          = delete; // Ngăn không cho sao chép
    void operator=(const Theme &) = delete; // Ngăn không cho gán

    QSettings _themes;
    QString   currentTheme;
};
