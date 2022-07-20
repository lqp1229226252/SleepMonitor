#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QLabel>
#include <QtSerialPort/QSerialPort>

#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class ControlWnd: public QWidget
{
    Q_OBJECT

public:
    ControlWnd(QWidget *parent = nullptr);
    ~ControlWnd();
public slots:

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
