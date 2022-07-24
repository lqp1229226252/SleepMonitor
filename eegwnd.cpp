#include "eegwnd.h"
#include "ui_eegwnd.h"
#include <QDebug>
#include <QTime>
EEGWnd::EEGWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EEGWnd)
{
    ui->setupUi(this);
    //初始化plotwidget指针容器
    init();
    //模拟设置16条曲线名称
    QVector<QString> names={
        "FP1脑电","FP2脑电","红光","近红光","绿光",
        "坐立角","翻滚角","运动加速度","左声道鼾声",
        "右声道鼾声","x-gro","y-gro","z-gro","x-acc",
        "y-acc","z-acc"
    };
    send_16PlotWidget_position();
    setLineName(names);
    //qDebug()<<names.size();

//    startTimer(50);

}

EEGWnd::~EEGWnd()
{
    //保存配置文件
    for(auto i:m_PlotWidgetPtrs)
    {
        i->SaveConfig();
        qDebug()<<"保存配置文件成功";
    }


    delete ui;
}
//接口
//依次调用内部PlotWidget实例的AddDatum，将data中的值传给各个PlotWidget实例
void EEGWnd::AddData(QVector<double> data)
{
    int i=0;
    for(auto PlotWidgetPtr : m_PlotWidgetPtrs)
    {
        PlotWidgetPtr->AddDatum(data[i++]);
    }
}

//调用内部各个PlotWidget实例的Initialize
void EEGWnd::Initialize(void)
{

}
//设置16条曲线名称
void EEGWnd::setLineName(QVector<QString> names)
{
    int i=0;
    for(auto PlotWidgetPtr : m_PlotWidgetPtrs)
    {
        PlotWidgetPtr->setLineName(names[i]);
        //qDebug()<<names[i];
        i++;
    }
}

//给每一个曲线发送他们自己在EEGWnd中的位置，在放大之后再恢复的时候可以找到自己的位置
void EEGWnd::send_16PlotWidget_position(void)
{

//    for(auto i : position)
//        qDebug()<<"x: "<<i.x()<<"  y: "<<i.y();
    //给每一个plotwidget发送他们自己最新相对位置
    int i=0;
    for(auto plotwidget:m_PlotWidgetPtrs)
    {
        //qDebug()<<"--"<<plotwidget_position.at(i);
        plotwidget->get_newest_position_from_EEGWnd(plotwidget_position.at(i));
        i++;
    }


    plotwidget_position.clear();


}
void EEGWnd::get_plotwidget_position(void)
{
    int i=0;
    for(auto plotwidget:m_PlotWidgetPtrs)
    {
        plotwidget_position.append( plotwidget->geometry());
    }
}
bool EEGWnd::event(QEvent *e)
{
    if (QEvent::WindowActivate == e->type())
        {
            //只要窗口一变就获取到最新的相对位置
            get_plotwidget_position();
            //发送给每一个widget最新的相对位置，
            send_16PlotWidget_position();
        }
         return QWidget::event(e);
}

//调用内部各个PlotWidget实例的SetValues
void EEGWnd::SetPlotAmount(int num)
{
    int i=0;
    for(auto PlotWidgetPtr : m_PlotWidgetPtrs)
    {
        PlotWidgetPtr->AddDatum(num);
    }
}

//定时器模拟输入数据

void EEGWnd::timerEvent(QTimerEvent *ev)
{
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
    QVector<double> data;
    for(int i=0;i<16;i++)
    {
        double value = qrand() % 10;

        data.append(value);
    }
    //qDebug()<<"data.size = "<<data.size();
    AddData(data);
}

//把EEGWnd窗口大小也传过去，最大化的时候根据EEGWnd的大小来最大化
void EEGWnd::send_EEGWnd_size(void)
{
   QSize size =  EEGWnd::size();
   qDebug()<<"EEGWnd"<<size.height();
   qDebug()<<"EEGWnd"<<size.width();
   for(auto plotwidget : m_PlotWidgetPtrs)
   {
       plotwidget->get_EEGWnd_Size(size);
   }
}

void EEGWnd::init(void)
{
    //初始化plotwidget指针容器
    m_PlotWidgetPtrs.append(ui->plotwidget1);
    m_PlotWidgetPtrs.append(ui->plotwidget2);
    m_PlotWidgetPtrs.append(ui->plotwidget3);
    m_PlotWidgetPtrs.append(ui->plotwidget4);
    m_PlotWidgetPtrs.append(ui->plotwidget5);
    m_PlotWidgetPtrs.append(ui->plotwidget6);
    m_PlotWidgetPtrs.append(ui->plotwidget7);
    m_PlotWidgetPtrs.append(ui->plotwidget8);
    m_PlotWidgetPtrs.append(ui->plotwidget9);
    m_PlotWidgetPtrs.append(ui->plotwidget10);
    m_PlotWidgetPtrs.append(ui->plotwidget11);
    m_PlotWidgetPtrs.append(ui->plotwidget12);
    m_PlotWidgetPtrs.append(ui->plotwidget13);
    m_PlotWidgetPtrs.append(ui->plotwidget14);
    m_PlotWidgetPtrs.append(ui->plotwidget15);
    m_PlotWidgetPtrs.append(ui->plotwidget16);
    //qDebug()<<"m_PlotWidgetPtrs.size = "<<m_PlotWidgetPtrs.size();

    //把这个容器传递个每一个plotwidget对象，以便在plotwidget点击放大恢复的时候来设置这些曲线是否可见
    int i=0;
    for(auto plotwidget : m_PlotWidgetPtrs)
    {
        plotwidget->from_eegwnd_QVector_PlotWidgets(m_PlotWidgetPtrs,i++);
    }

    send_EEGWnd_size();

}
