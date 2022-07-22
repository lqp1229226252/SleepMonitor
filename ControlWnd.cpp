#include "ControlWnd.h"
#include "ui_ControlWnd.h"
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

ControlWnd::ControlWnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    setWindowTitle(tr("控制窗口"));//设置标题

    ui->widget_contralData->addWidget(contralDataWidget);
    ui->widget_SleepInterven->addWidget(sleepinterventionwidget);
    ui->widget_comm->addWidget(commsetwidget);

    connect(sleepinterventionwidget,SIGNAL(sendSignal(QByteArray)),this,SLOT(send_command(QByteArray)));
    connect(contralDataWidget,SIGNAL(sendSingal(QByteArray)),this,SLOT(send_command(QByteArray)));
    connect(commsetwidget,SIGNAL(dataSignal(QVector<double>)),this,SLOT(dataPlot(QVector<double>)));

}

void ControlWnd::dataPlot(QVector<double> data){
    qDebug()<<"ControlWnd::dataPlot"<<data;
    eegwnd->AddData(data);
}

void ControlWnd::send_command(QByteArray data) {
    commsetwidget->write(data);
}

void ControlWnd::get_eegwnd(EEGWnd *eegwnd,OutPutWnd *outputwnd){
    this->eegwnd=eegwnd;
//    commsetwidget->get_eegwnd(eegwnd,outputwnd);
}


ControlWnd::~ControlWnd()
{
    delete sleepinterventionwidget;
    delete contralDataWidget;
    delete ui;
}

