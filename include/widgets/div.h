#ifndef DIV_H
#define DIV_H

#include <QFrame>
#include <QLayout>
#include <QString>
#include <QWidget>

class HDiv : public QWidget
{
    Q_OBJECT

public:
    explicit HDiv(QWidget *parent = nullptr);
};

class VDiv : public QWidget
{
    Q_OBJECT

public:
    explicit VDiv(QWidget *parent = nullptr);
};

#endif // DIV_H
