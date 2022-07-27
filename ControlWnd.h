#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QLabel>
#include <QtSerialPort/QSerialPort>

#include "eegwnd.h"
#include "outputwnd.h"
#include "ContralDataWidget.h"
#include "sleepinterventionwidget.h"
#include "commsetwidget.h"
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
    void get_eegwnd(EEGWnd *,OutPutWnd *);
    void FPOverFlowSlot();
    void lightOverFlowSlot();
    void angleaccOverFlowSlot();
    void snoreOverFlowSlot();
    void groaccOverFlowSlot();


signals:


private:
    Ui::Widget *ui;

    SleepInterventionWidget *sleepinterventionwidget=new SleepInterventionWidget();
    ContralDataWidget *contralDataWidget=new ContralDataWidget();
    CommSetWidget *commsetwidget=new CommSetWidget();
    EEGWnd *eegwnd;

private slots:
    void send_command(QByteArray);
    void dataPlot(QVector<double>);

};

#endif // WIDGET_H
