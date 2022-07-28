#include "plotwidget.h"
#include "ui_plotwidget.h"


PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);
    //点击属性按钮，显示和隐藏属性表
    //connect(ui->attribute,&QPushButton::clicked,this,&PlotWidget::clicked_attribute);

    PlotWidget_size = PlotWidget::size();
    //点击最大化按钮，放大这个窗口并且隐藏其他曲线
    connect(ui->maxmum,&QPushButton::clicked,this,&PlotWidget::clicked_maxmum);
    // startTimer(50);
   // ui->attribute->setVisible(false);
    //从dynamicplot类中读取曲线的perporty属性
    //updata_property_struct();

    //设置按钮icon
    QIcon icon_maxmum;
    icon_maxmum.addFile(tr(":/image/icon/1.ico"));

    ui->maxmum->setIcon(icon_maxmum);
    ui->maxmum->setIconSize(QSize(35,35));

    ui->maxmum->setFlat(true);
}

//从配置文件中读内容到property结构体中
void PlotWidget::ReadConfig(void)
{
    QString appPath = QDir::currentPath();
    QString dir_str = appPath;
    dir_str += "/config";
    QString plotname = "/plotwidget";
    plotname += QString::number(this->id);
    plotname += ".ini";

    QString tempFilePath = dir_str + plotname;
//    qDebug()<<tempFilePath;
    QSettings settings(tempFilePath,QSettings::IniFormat);
    m_Property.background0 = stringToColor( settings.value("background0").toString());
    m_Property.background1 = stringToColor( settings.value("background1").toString());
    m_Property.xBasePen =  stringToColor( settings.value("xBasePen").toString());
    m_Property.yBasePen = stringToColor( settings.value("yBasePen").toString());
    m_Property.basePenWidth = ( settings.value("basePenWidth").toInt());
    m_Property.tickPen =  stringToColor( settings.value("tickPen").toString());
    m_Property.labelPen = stringToColor( settings.value("labelPen").toString());
    m_Property.labelSize = ( settings.value("labelSize").toInt());
    m_Property.xGridPen =  stringToColor( settings.value("xGridPen").toString());
    m_Property.yGridPen = stringToColor( settings.value("yGridPen").toString());
    m_Property.xSubGridVisible = ( settings.value("xSubGridVisible").toBool());
    m_Property.ySubGridVisible = ( settings.value("xSubGridVisible").toBool());
    m_Property.xSubGridPen =  stringToColor( settings.value("xSubGridPen").toString());
    m_Property.ySubGridPen = stringToColor( settings.value("ySubGridPen").toString());
    m_Property.gridPenStyle = Qt::PenStyle( settings.value("gridPenStyle").toInt());
    m_Property.subGridPenStyle = Qt::PenStyle( settings.value("subGridPenStyle").toInt());
    m_Property.zeroLinePen = stringToColor( settings.value("zeroLinePen").toString());
    m_Property.xAxisName = settings.value("xAxisName").toString();
    m_Property.yAxisName = settings.value("yAxisName").toString();
    m_Property.xAxisUnit = settings.value("xAxisUnit").toString();
    m_Property.yAxisUnit = settings.value("yAxisUnit").toString();
    m_Property.xAxisUpper = settings.value("xAxisUpper").toDouble();
    m_Property.xAxisLower = settings.value("xAxisLower").toDouble();
    m_Property.yAxisUpper = settings.value("yAxisUpper").toDouble();
    m_Property.yAxisLower = settings.value("yAxisLower").toDouble();
    m_Property.linesName = settings.value("linesName").toString();
    m_Property.lineVisible = settings.value("lineVisible").toBool();
    m_Property.linesColor = stringToColor( settings.value("linesColor").toString());
    m_Property.linesWidth = settings.value("linesWidth").toInt();
    m_Property.linePenStyle = Qt::PenStyle( settings.value("linePenStyle").toInt());
    m_Property.lineStyle = QCPGraph::LineStyle(settings.value("lineStyle").toInt());
    m_Property.lineScatterStyle = QCPScatterStyle::ScatterShape(settings.value("lineScatterStyle").toInt());
    //qDebug()<<"read config:"<<m_Property.linesName;

    //readconfig之后调用dynamicplot类中的upada更新曲线,把结构体传过去更新
    ui->dynamicplot->update_Axis_line_enum_properties(&m_Property);

}

//把perporty结构体中的数据保存在ini文件中
void PlotWidget::SaveConfig(void)
{
    //首先获取程序的运行目录
    QString appPath = QDir::currentPath();

    QString dir_str = appPath;
    dir_str += "/config";
    QDir dir;
    // 检查目录是否存在，若不存在则新建
    if(!dir.exists(dir_str))
    {
        bool res = dir.mkpath(dir_str);
    }

    QString plotname = "/plotwidget";
    plotname += QString::number(this->id);
    plotname += ".ini";
    QString tempFilePath = dir_str + plotname;  // currentPath()方法返回应用程序所在的当前目录；
    QSettings settings(tempFilePath,QSettings::Format::IniFormat);


    settings.setValue("background0",colorToString(m_Property.background0));
    settings.setValue("background1",colorToString(m_Property.background1));
    settings.setValue("xBasePen",colorToString(m_Property.xBasePen));
    settings.setValue("yBasePen",colorToString(m_Property.yBasePen));
    settings.setValue("basePenWidth",m_Property.basePenWidth);

    settings.setValue("tickPen",colorToString(m_Property.tickPen));
    settings.setValue("labelPen",colorToString(m_Property.labelPen));
    settings.setValue("labelSize",m_Property.labelSize);
    settings.setValue("xGridPen",colorToString(m_Property.xGridPen));
    settings.setValue("yGridPen",colorToString(m_Property.yGridPen));
    settings.setValue("xSubGridVisible",m_Property.xSubGridVisible);
    settings.setValue("ySubGridVisible",m_Property.ySubGridVisible);

    settings.setValue("xSubGridPen",colorToString(m_Property.xSubGridPen));
    settings.setValue("ySubGridPen",colorToString(m_Property.ySubGridPen));
    QVariant var;
    var.setValue(m_Property.gridPenStyle);
    settings.setValue("gridPenStyle",var.toInt());
    var.setValue(m_Property.subGridPenStyle);
    settings.setValue("subGridPenStyle",var.toInt());
    settings.setValue("zeroLinePen",colorToString(m_Property.zeroLinePen));
    settings.setValue("xAxisName",m_Property.xAxisName);
    settings.setValue("yAxisName",m_Property.yAxisName);
    settings.setValue("xAxisUnit",m_Property.xAxisUnit);
    settings.setValue("yAxisUnit",m_Property.yAxisUnit);
    settings.setValue("xAxisUpper",m_Property.xAxisUpper);
    settings.setValue("xAxisLower",m_Property.xAxisLower);
    settings.setValue("yAxisUpper",m_Property.yAxisUpper);
    settings.setValue("yAxisLower",m_Property.yAxisLower);
    settings.setValue("linesName",m_Property.linesName);
    settings.setValue("lineVisible",m_Property.lineVisible);
    settings.setValue("linesColor",colorToString(m_Property.linesColor));
    settings.setValue("linesWidth",m_Property.linesWidth);
    var.setValue(m_Property.linePenStyle);

    settings.setValue("linePenStyle",var.toInt());
    settings.setValue("lineStyle",m_Property.lineStyle);
    settings.setValue("lineScatterStyle",m_Property.lineScatterStyle);

}
QString PlotWidget::colorToString(const QColor color)
{
    QRgb mRgb = qRgb(color.red(), color.green(), color.blue());
    return QString::number(mRgb, 16);
}

QColor PlotWidget::stringToColor(const QString str)
{
    return QColor(str.toUInt(nullptr, 16));
}
//把当前曲线的信息读到结构体中
void PlotWidget::updata_property_struct(void)
{
    ui->dynamicplot->update_property_struct(&(this->m_Property));
    //qDebug()<<"updata_property_struct"<<m_Property.linesName;
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
    //设置完曲线名字后再把属性值读到struct中，不然没有曲线名字

    updata_property_struct();

    //创建存放属性文件夹 在这调用是因为要用id来命令文件夹
//    qDebug()<<"linesName :"<<m_Property.linesName;
    //设置完名字后把属性结构体保存在配置文件中
    //如果config文件不存在则先saveconfig把结构体默认值（也就是dynamicplot类中初始化曲线的参数）
    //先保存在结构体中并且存到配置文件中再readconfig，否则就不执行saveconfig这样就解决了
    //程序第一次运行的时候使用默认值而非第一次运行时使用配置文件的问题
    //先判断是否存在配置文件，若不存在则执行saveconfig
    //首先获取程序的运行目录
    QString appPath = QDir::currentPath();
    QString dir_str = appPath;
    dir_str += "/config";
    QString plotname = "/plotwidget";
    plotname += QString::number(this->id);
    plotname += ".ini";
    QString tempFilePath = dir_str + plotname;
    QFileInfo fi(tempFilePath);
    //文件不存在的情况
    if(!fi.exists())
    {
        qDebug()<<"程序首次运行，无配置文件";
        SaveConfig();
    }


    ReadConfig();


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
    //qDebug()<<"曲线："<<id<<"最新相对位置 "<<"x= "<<position_in_EEGWnd.x()<<" y= "<<position_in_EEGWnd.y();
    //用这个最新的位置来点击恢复的时候用
}

//Slot:点击最大化按钮
void PlotWidget::clicked_maxmum(void)
{
    //qDebug()<<id<<" clicked";
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
        //ui->attribute->setVisible(false);
        ui->maxmum->setVisible(false);
    }

    else
    {
        ui->dynamicplot->setVisible(true);
        //ui->attribute->setVisible(false);
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
    //this->ui->attribute->setVisible(false);
    //this->ui->maxmum->setText("恢复");

    this->setGeometry(0,0,EEGWnd_size.width(), EEGWnd_size.height());
    this->setFixedHeight(EEGWnd_size.height());
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
    //this->ui->attribute->setVisible(false);
    //this->ui->maxmum->setText("放大");

    //用自己在EEGWnd中的位置来恢复放大之前的位置
    this->setGeometry(position_in_EEGWnd.x(),position_in_EEGWnd.y(),position_in_EEGWnd.width(),position_in_EEGWnd.height());
    this->setFixedHeight(PlotWidget_size.height());
    this->activateWindow();
    //恢复之后要隐藏坐标轴
    ui->dynamicplot->setXYvisbile(false);
}

PlotWidget::~PlotWidget()
{
    //任务19内容5 保存属性信息到配置文件中
    //SaveConfig();
    //qDebug()<<"plotwidget " <<this->id<<"保存配置文件成功";
    delete ui;
}
