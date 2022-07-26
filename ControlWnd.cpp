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
//    qDebug()<<"ControlWnd::dataPlot"<<data;
    int fp_num=contralDataWidget->getCtrlData()->getFPAmount();
    int snore_num=contralDataWidget->getCtrlData()->getSnoreAmount();
    int light_num=contralDataWidget->getCtrlData()->getLightAmount();
    int acc_num=contralDataWidget->getCtrlData()->getGroAccAmount();
    QList<int> list;
    list.append(fp_num);
    list.append(snore_num);
    list.append(light_num);
    list.append(acc_num);
    qSort(list.begin(),list.end());
    int max_num=list.last();
    int FP_stride=max_num/fp_num;
    int light_stride=max_num/light_num;
    int snore_stride=max_num/snore_num;
    int groacc_stride=max_num/acc_num;
    int fp_index=-1;
    int light_index=fp_index+2*fp_num;
    int angleacc_index=light_index+light_num*3;
    int snore_index=angleacc_index+3;
    int groacc_index=snore_index+snore_num*2;
    for(int i=0;i<max_num;i++)
    {
        if(i%FP_stride==0)
        {
            fp_index++;
        }
        if(i%light_stride==0)
        {
            light_index++;
        }
        if(i%snore_stride==0)
        {
            snore_index++;
        }
        if(i%groacc_stride==0)
        {
            groacc_index++;
        }
        QVector<double> chart_data;
        chart_data.append(data[fp_index]);
        chart_data.append(data[fp_index+fp_num]);

        chart_data.append(data[light_index]);
        chart_data.append(data[light_index+light_num]);
        chart_data.append(data[light_index+light_num*2]);

        chart_data.append(data[angleacc_index]);
        chart_data.append(data[angleacc_index+1]);
        chart_data.append(data[angleacc_index+2]);

        chart_data.append(data[snore_index]);
        chart_data.append(data[snore_index+snore_num]);

        chart_data.append(data[groacc_index]);
        chart_data.append(data[groacc_index+acc_num]);
        chart_data.append(data[groacc_index+acc_num*2]);
        chart_data.append(data[groacc_index+acc_num*3]);
        chart_data.append(data[groacc_index+acc_num*4]);
        chart_data.append(data[groacc_index+acc_num*5]);

        eegwnd->AddData(chart_data);
    }

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

