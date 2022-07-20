#include "plotwidget.h"
#include "ui_plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
    //点击属性按钮，显示和隐藏属性表
    connect(ui->attribute,&QPushButton::clicked,this,&PlotWidget::clicked_attribute);

    PlotWidget_size = PlotWidget::size();
    //点击最大化按钮，放大这个窗口并且隐藏其他曲线
    connect(ui->maxmum,&QPushButton::clicked,this,&PlotWidget::clicked_maxmum);
    // startTimer(50);
    ui->attribute->setVisible(false);

}
//三个接口
void PlotWidget::Initialize(void)
{
    ui->dynamicplot->Initialize();
}
void PlotWidget::SetValues (double val)
{
    ui->dynamicplot->SetValues(val);
}
void PlotWidget::AddDatum(double val)
{
    ui->dynamicplot->AddDatum(val);
}
//设置曲线名称
void PlotWidget::setLineName(QString name)
{
    ui->dynamicplot->setLineName(name);
}
void PlotWidget::timerEvent(QTimerEvent *event)
{

    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);

    double value = qrand() % 10;

    AddDatum(value);


}
//接收来自EEGWnd类中传来的16条曲线对应的plotwidget对象的引用，在点击最大化的时候来放大自己这个对象
//设置其他曲线是否可见
void PlotWidget::from_eegwnd_QVector_PlotWidgets(QVector<PlotWidget*> pltwidgets,int id)
{
    this->id = id;
    this->plotwidgets = pltwidgets;

}
//获取EEGWnd窗口大小函数
void PlotWidget::get_EEGWnd_Size(QSize size)
{
    this->EEGWnd_size = size;
}

//获取自己最新的相对位置
void PlotWidget::get_newest_position_from_EEGWnd(QRect rect)
{
    this->position_in_EEGWnd = rect;
    qDebug()<<"曲线："<<id<<"最新相对位置 "<<"x= "<<position_in_EEGWnd.x()<<" y= "<<position_in_EEGWnd.y();
    //用这个最新的位置来点击恢复的时候用
}

//Slot:点击最大化按钮
void PlotWidget::clicked_maxmum(void)
{
    qDebug()<<id<<" clicked";
    //知道自己是第几条曲线了就能达到放大自己不显示其他曲线了
    //放大自己，隐藏别人
    zoomPlotWidget();

}
//Slot:点击属性按钮
void PlotWidget::clicked_attribute(void)
{
    //是否显示属性窗
    this->ui->dynamicplot->setTableVisible();

    //放大自己                    宽    高
    //PlotWidget::setGeometry(0,0,EEGWnd_size.width()-10, EEGWnd_size.height());
    PlotWidget::setGeometry(0,0,EEGWnd_size.width(), EEGWnd_size.height());
    PlotWidget::activateWindow();
    ui->dynamicplot->resize(EEGWnd_size.width()-80,EEGWnd_size.height());

    //ui->dynamicplot->setGeometry(0,0,EEGWnd_size.width()-40, EEGWnd_size.height());
    ui->dynamicplot->activateWindow();

}
void PlotWidget::hide_plotWidget(bool f)
{
    if(f==true)
    {
        ui->dynamicplot->setVisible(false);
        //隐藏按钮
        ui->attribute->setVisible(false);
        ui->maxmum->setVisible(false);
    }

    else
    {
        ui->dynamicplot->setVisible(true);
        ui->attribute->setVisible(false);
        ui->maxmum->setVisible(true);
    }

}
void PlotWidget::zoomPlotWidget(void)
{
    if(flag_maxmum==false) //不是最大化标志那就最大化这个窗口
    {
        Max_Widget();
        flag_maxmum=true;
    }
    else //是最大化标志那就复原这个窗口
    {
        Recovery_Widget();
        flag_maxmum=false;
    }

}
void PlotWidget::Max_Widget(void)
{
    //隐藏其他
    int i=0;
    for(auto tmp_plot:plotwidgets)
    {
        //如果是自己那就跳过，只隐藏其他曲线
        if(i==this->id)
        {
            i++;
            continue;
        }
        tmp_plot->hide_plotWidget(true);
        i++;
    }
    //设置属性可见

    //默认显示属性页面
    ui->dynamicplot->setTableVisible();
    //默认显示属性页面，隐藏属性按钮
    this->ui->attribute->setVisible(false);
    this->ui->maxmum->setText("恢复");

    this->setGeometry(0,0,EEGWnd_size.width(), EEGWnd_size.height());
    this->activateWindow();

    //放大之后要显示坐标轴
    ui->dynamicplot->setXYvisbile(true);

}
//恢复窗口  需要知道自己在EEGWnd中的位置才行
void PlotWidget::Recovery_Widget(void)
{
    //显示其他
    int i=0;
    for(auto tmp_plot:plotwidgets)
    {
        //如果是自己那就跳过，只隐藏其他曲线
        if(i==this->id)
        {
            i++;
            continue;
        }
        tmp_plot->hide_plotWidget(false);
        i++;
    }
    //设置属性可见

    //默认显示属性页面
    ui->dynamicplot->setTableVisible();
    //默认显示属性页面，隐藏属性按钮
    this->ui->attribute->setVisible(false);
    this->ui->maxmum->setText("放大");

    //用自己在EEGWnd中的位置来恢复放大之前的位置
    this->setGeometry(position_in_EEGWnd.x(),position_in_EEGWnd.y(),position_in_EEGWnd.width(),position_in_EEGWnd.height());
    this->activateWindow();
    //恢复之后要隐藏坐标轴
    ui->dynamicplot->setXYvisbile(false);
}

PlotWidget::~PlotWidget()
{
    delete ui;
}
