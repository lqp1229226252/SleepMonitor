#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QLabel>
#include <QtSerialPort/QSerialPort>

#include "curvegroup.h"
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
    void  setSaveConnect();
public slots:
    void get_eegwnd(CurveGroup *,OutPutWnd *);

    void FP1OverFlowSlot();
    void FP2OverFlowSlot();

    void redLightOverFlowSlot();
    void nearRedLightOverFlowSlot();
    void greenLightOverFlowSlot();

    void seatAngleOverFlowSlot();
    void rollAngleOverFlowSlot();
    void AccOverFlowSlot();

    void LSnoreOverFlowSlot();
    void RSnoreOverFlowSlot();

    void groXOverFlowSlot();
    void groYOverFlowSlot();
    void groZOverFlowSlot();

    void accXOverFlowSlot();
    void accYOverFlowSlot();
    void accZOverFlowSlot();



signals:


private:
    Ui::Widget *ui;

    SleepInterventionWidget *sleepinterventionwidget;
    ContralDataWidget *contralDataWidget;
    CommSetWidget *commsetwidget;
    CurveGroup *curvegroup;

private slots:
    void send_command(QByteArray);
    void dataPlot(QVector<double>);

};

#endif // WIDGET_H
