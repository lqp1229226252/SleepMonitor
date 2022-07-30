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

    connect(commsetwidget,SIGNAL(fileStart(QString)),contralDataWidget->getCtrlData(),SLOT(SaveBegin(QString)));//文件转存信号与ctrldata的savebegin连接
    connect(commsetwidget,SIGNAL(fileEnd()),contralDataWidget->getCtrlData(),SLOT(SaveEnd()));//文件转存结束信号

    //设置文件信号的连接
    setSaveConnect();
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

    ControlData *ctrl_data=contralDataWidget->getCtrlData();
    SensorData *sensordata=commsetwidget->getSensorData();
    sensordata->setCtrlData(ctrl_data->getFPAmount(),ctrl_data->getLightAmount(),ctrl_data->getSnoreAmount(),ctrl_data->getGroAccAmount(),ctrl_data->getCtrlData().nSrc);
    commsetwidget->write(data);
}

void ControlWnd::get_eegwnd(EEGWnd *eegwnd,OutPutWnd *outputwnd){
    this->eegwnd=eegwnd;
    //    commsetwidget->get_eegwnd(eegwnd,outputwnd);
}

void ControlWnd::FP1OverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_FP> FP1=data->getFP1();
       //将数据存储在csv
       ctrl->saveFP1(FP1);
    }
}

void ControlWnd::FP2OverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_FP> FP2=data->getFP2();
       //将数据存储在csv
       ctrl->saveFP2(FP2);
    //           qDebug()<<"转存完毕";
    }
}

void ControlWnd::redLightOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_LIGTH> red_light=data->getRedLight();
       //将数据存储在csv
       ctrl->saveRedLight(red_light);
    }
}

void ControlWnd::nearRedLightOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_LIGTH> near_red_light=data->getNearReadLight();
       //将数据存储在csv
       ctrl->saveRedNearLight(near_red_light);
    }

}
void ControlWnd::greenLightOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_LIGTH> green_light=data->getGreenLight();
       //将数据存储在csv
       ctrl->saveGreenLight(green_light);;
    }
}
void ControlWnd::seatAngleOverFlowSlot()
{

    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<double> seat_angle=data->getSeatAngle();
       //将数据存储在csv
       ctrl->saveSeatAngle(seat_angle);
    }
}
void ControlWnd::rollAngleOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<double> roll_angle=data->getRollAngel();
       //将数据存储在csv
       ctrl->saveRollAngel(roll_angle);
    }
}
void ControlWnd::AccOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<double> acc=data->getActionAcc();
       //将数据存储在csv
       ctrl->saveAcc(acc);
    }
}
void ControlWnd::LSnoreOverFlowSlot()
{

    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_SNORE> l_snore=data->getSnoreLeft();
       //将数据存储在csv
       ctrl->saveLSnore(l_snore);
    }
}
void ControlWnd::RSnoreOverFlowSlot()
{

    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_SNORE> r_snore=data->getSnoreRight();
       //将数据存储在csv
       ctrl->saveRSnore(r_snore);
    }
}
void ControlWnd::groXOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_GRO> grox=data->getGrox();
       //将数据存储在csv
       ctrl->saveGrox(grox);
    }
}
void ControlWnd::groYOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_GRO> groy=data->getGroy();
       //将数据存储在csv
       ctrl->saveGroy(groy);
    }
}
void ControlWnd::groZOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_GRO> groz=data->getGroz();
       //将数据存储在csv
       ctrl->saveGroz(groz);
    }
}
void ControlWnd::accXOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_ACC> accx=data->getAccx();
       //将数据存储在csv
       ctrl->saveACCx(accx);
    }
}
void ControlWnd::accYOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_ACC> accy=data->getAccy();
       //将数据存储在csv
       ctrl->saveACCy(accy);
    }
}
void ControlWnd::accZOverFlowSlot()
{
    ControlData *ctrl=contralDataWidget->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
       //获取数据
       SensorData *data=commsetwidget->getSensorData();
       QVector<SD_ACC> accz=data->getAccz();
       //将数据存储在csv
       ctrl->saveAccz(accz);
    }
}
ControlWnd::~ControlWnd()
{
    delete sleepinterventionwidget;
    delete contralDataWidget;
    delete ui;
}

void ControlWnd::setSaveConnect()
{
    connect(commsetwidget->getSensorData(),SIGNAL(FP1OverFlow()),this,SLOT(FP1OverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(FP2OverFlow()),this,SLOT(FP2OverFlowSlot()));

    connect(commsetwidget->getSensorData(),SIGNAL(redLightOverFlow()),this,SLOT(redLightOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(nearRedLightOverFlow()),this,SLOT(nearRedLightOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(greenLightOverFlow()),this,SLOT(greenLightOverFlowSlot()));

    connect(commsetwidget->getSensorData(),SIGNAL(seatAngleOverFlow()),this,SLOT(seatAngleOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(rollAngleOverFlow()),this,SLOT(rollAngleOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(accOverFlow()),this,SLOT(AccOverFlowSlot()));

    connect(commsetwidget->getSensorData(),SIGNAL(lSnoreOverFlow()),this,SLOT(LSnoreOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(rSnoreOverFlow()),this,SLOT(RSnoreOverFlowSlot()));

    connect(commsetwidget->getSensorData(),SIGNAL(GroXOverFlow()),this,SLOT(groXOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(GroYOverFlow()),this,SLOT(groYOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(GroZOverFlow()),this,SLOT(groZOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(ACCXOverFlow()),this,SLOT(accXOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(ACCYOverFlow()),this,SLOT(accYOverFlowSlot()));
    connect(commsetwidget->getSensorData(),SIGNAL(ACCZOverFlow()),this,SLOT(accZOverFlowSlot()));
}

