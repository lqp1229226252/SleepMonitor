#include "dynamicplot.h"
#include "ui_dynamicplot.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMetaEnum>
#include <QDebug>

DynamicPlot::DynamicPlot(QWidget *parent, const QString fileName) :
    QWidget(parent),
    isMagnify(false),
    ui(new Ui::DynamicPlot),
    magnifyForNewWindow(false),
    fileName(fileName),
    styleFileName(""),
    xAxisUpper(50),//INT_MIN
    xAxisLower(0),//INT_MAX
    yAxisUpper(10),
    yAxisLower(0),
    //设置画布背景颜色
    background0(QColor(240, 240, 240)),
    background1(QColor(240, 240, 240)),

    xGridPen(QColor(140, 140, 140)),
    yGridPen(QColor(140, 140, 140)),
    xSubGridPen(QColor(80, 80, 80)),
    ySubGridPen(QColor(80, 80, 80)),
    gridPenStyle(Qt::PenStyle::DotLine),
    subGridPenStyle(Qt::PenStyle::DotLine),
    //设置网格线是否可见
    xSubGridVisible(false),
    ySubGridVisible(false),


    zeroLinePen(Qt::red),
    xBasePen(Qt::red),
    yBasePen(Qt::red),
    basePenWidth(2),
    tickPen(Qt::white),
    labelPen(Qt::white),
    labelSize(9),
    xAxisName("x"),
    yAxisName("y"),
    // Lines
    xAxisUnit("cm"),
    yAxisUnit("cm"),
    linesNum(0),
    curLineIndex(-1),
    scatterSize(10)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    //右侧工具栏，注释了就不显示
    //setToolBar();
    //先读数据后调用drawing绘图
    //readData();


    //if (!data.empty())
    linesNum=1;

    init();


    //QVector<QVector<QCPGraphData>> data;

    //calculatesPropertyValue(data, curveProperties);
    //getXYLowerAndUpper(data);

    myCustomPlot = ui->plotWidget;

    //初始化曲线

    Initialize();
    //drawing(myCustomPlot);

    //设置属性表
    //setAxisProperties();
    //所有的属性都存在QMap<QString, T> styleFileVariables
    setStyleFileVariables();
    //------------------------------
    side_widget_flag = false;
    buttonRelesed = true;
    tableAnimation = new QPropertyAnimation(ui->tableWidget,"geometry");
    tableAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    tableAnimation->setDuration(1000);
    plotAnimation = new QPropertyAnimation(ui->plotWidget,"geometry");
    plotAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    plotAnimation->setDuration(1000);

    //开始的时候右侧属性表不可见
    ui->tableWidget->setVisible(false);
    connect(plotAnimation, &QPropertyAnimation::finished, this, &DynamicPlot::animationFinished);




    startTimer(1);


}
//更新plotwidget中的结构体
void DynamicPlot::update_property_struct(PLOT_PROPERTY *p)
{
    p->background0 = this->background0;
    p->background1 = this->background1;
    p->xBasePen = this->xBasePen;
    p->yBasePen = this->yBasePen;
    p->basePenWidth=this->basePenWidth;
    p->tickPen = this->tickPen;
    p->labelPen=this->labelPen;
    p->labelSize=this->labelSize;
    p->xGridPen=this->xGridPen;
    p->yGridPen=this->yGridPen;
    p->xSubGridPen=this->xSubGridPen;
    p->ySubGridPen=this->ySubGridPen;
    p->xSubGridVisible=this->xSubGridVisible;
    p->ySubGridVisible=this->ySubGridVisible;
    p->gridPenStyle=this->gridPenStyle;
    p->subGridPenStyle=this->subGridPenStyle;
    p->zeroLinePen=this->zeroLinePen;
    p->xAxisName=this->xAxisName;
    p->yAxisName=this->yAxisName;
    p->xAxisUnit=this->xAxisUnit;
    p->yAxisUnit=this->yAxisUnit;
    p->xAxisUpper=this->xAxisUpper;
    p->yAxisUpper=this->yAxisUpper;
    p->xAxisLower=this->xAxisLower;
    p->yAxisLower=this->yAxisLower;
    //p->linesName=this->line_name;
    p->linesName = this->linesName[0];
    p->lineVisible=this->lineVisible[0];
    p->linesColor=this->linesColor[0];
    p->linesWidth=this->linesWidth[0];
    p->linePenStyle=this->linePenStyle[0];
    p->lineStyle=this->lineStyle[0];
    p->lineScatterStyle=this->lineScatterStyle[0];

    //因为在dynamicplot中属性会改变所以dynamicplot类中必须要这个指针随时更新
    this->my_plot_property = p;


}
void DynamicPlot::setTableVisible()
{

    //是否打开窗口槽函数
    if(flag_setTableVisible==false)
    {
        //ui->tableWidget->setVisible(true);
        ui->tableWidget->show();

        flag_setTableVisible=true;
    }
    else
    {
        //ui->tableWidget->setVisible(false);
        ui->tableWidget->hide();
        flag_setTableVisible=false;
    }



}

void DynamicPlot::timerEvent(QTimerEvent *event)
{


    //   qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);

    //    double value = qrand() % 100;

    //定时器只生成一个x坐标，y坐标通过上级的类通过addDatum来传递
    sampleNumber+=1;

    //AddDatum(value);


}

//接口-------------------------------------------------------

//将曲线y坐标全部设置为val
void DynamicPlot::SetValues(double val)
{
    ui->plotWidget->graph(0)->addData(sampleNumber,val);

    //设置x轴不断更新函数原型
    //void QCPAxis::setRange(double position,doublesize,Qt::AlignmentFlag alignment)
    //参数含义：position定位的点 doublesize轴的范围大小 alignment对齐方式
    //例如下面这个setRange(sampleNumber, 50, Qt::AlignRight);
    //就解释为：新生成的x这个点一直在右边框，这样就可以一直刷新x轴了
    ui->plotWidget->xAxis->setRange(sampleNumber, this->xAxisUpper, Qt::AlignRight);

    ui->plotWidget->replot();
}


//传入新的一个数据点然后调用replot更新曲线
void DynamicPlot::AddDatum(double val)
{

    //

    ui->plotWidget->graph(0)->addData(sampleNumber,val);

    //设置x轴不断更新函数原型
    //void QCPAxis::setRange(double position,doublesize,Qt::AlignmentFlag alignment)
    //参数含义：position定位的点 doublesize轴的范围大小 alignment对齐方式
    //例如下面这个setRange(sampleNumber, 50, Qt::AlignRight);
    //就解释为：新生成的x这个点一直在右边框，这样就可以一直刷新x轴了
    ui->plotWidget->xAxis->setRange(sampleNumber, this->xAxisUpper, Qt::AlignRight);

    ui->plotWidget->replot();
}
//添加曲线名称接口
void DynamicPlot::setLineName(QString name)
{
    this->line_name = name;
    this->linesName[0]=name;
    myGraph[0]->setName(name);
}
//初始化曲线各种属性
void DynamicPlot::Initialize(void)
{
    drawing_Initialize(myCustomPlot);
}
void DynamicPlot::drawing_Initialize(QCustomPlot* customPlot)
{
    //初始化曲线颜色等属性
    initDrawing();

    //添加曲线且设置曲线颜色等属性
    addLineGraph(customPlot,  0);


    //设置坐标轴的显示范围
    customPlot->xAxis->setRange(0, xAxisUpper);
    customPlot->yAxis->setRange(0, yAxisUpper);

    //设置画布背景颜色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, background0);
    plotGradient.setColorAt(1, background1);
    customPlot->setBackground(plotGradient);


    //设置坐标轴刻度线的风格
    customPlot->xAxis->setBasePen(QPen(xBasePen, basePenWidth));         //轴线的画笔
    customPlot->xAxis->setTickPen(QPen(tickPen, 2));         //轴刻度线的画笔
    customPlot->xAxis->setSubTickPen(QPen(tickPen, 1));      //轴子刻度线的画笔
    customPlot->xAxis->setTickLabelColor(labelPen);           //轴刻度线文字颜色
    //    customPlot->xAxis->setLabel(xAxisName + "(" + xAxisUnit + ")");//只有设置了标签，轴标签的颜色才会显示
    //    customPlot->xAxis->setLabelColor(labelPen);
    customPlot->xAxis->setTickLengthIn(3);                     //轴线内刻度的长度
    customPlot->xAxis->setTickLengthOut(5);                    //轴线外刻度的长度
    customPlot->xAxis->setUpperEnding((QCPLineEnding::esSpikeArrow));  //设置轴线结束时的风格，setLowerEnding设置轴线开始时的风格
    customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);//可读性优于设置
    customPlot->xAxis->setNumberFormat("gbc");//g灵活的格式,b漂亮的指数形式，c乘号改成×
    customPlot->yAxis->setBasePen(QPen(yBasePen, basePenWidth));         //轴线的画笔
    customPlot->yAxis->setTickPen(QPen(tickPen, 2));         //轴刻度线的画笔
    customPlot->yAxis->setSubTickPen(QPen(tickPen, 1));      //轴子刻度线的画笔
    customPlot->yAxis->setTickLabelColor(labelPen);           //轴刻度线文字颜色
    //    customPlot->yAxis->setLabel(yAxisName + "(" + yAxisUnit + ")");//只有设置了标签，轴标签的颜色才会显示
    //    customPlot->yAxis->setLabelColor(labelPen);
    customPlot->yAxis->setTickLengthIn(3);                     //轴线内刻度的长度
    customPlot->yAxis->setTickLengthOut(5);                    //轴线外刻度的长度
    customPlot->yAxis->setUpperEnding((QCPLineEnding::esSpikeArrow));  //设置轴线结束时的风格，setLowerEnding设置轴线开始时的风格
    customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);//可读性优于设置
    customPlot->yAxis->setNumberFormat("gbc");//g灵活的格式,b漂亮的指数形式，c乘号改成×

    // 子刻度线，每条网格对应一个刻度
    customPlot->xAxis->grid()->setPen(QPen(xGridPen, 2, gridPenStyle));     // 网格线(对应刻度)画笔

    //！！！x轴网格线可见
    customPlot->xAxis->grid()->setVisible(false);     // 网格线(对应刻度)画笔
    customPlot->xAxis->grid()->setSubGridPen(QPen(xSubGridPen, 1, subGridPenStyle)); // 子网格线(对应子刻度)画笔
    customPlot->xAxis->grid()->setSubGridVisible(xSubGridVisible);     // 显示子网格线
    customPlot->xAxis->grid()->setZeroLinePen(QPen(zeroLinePen));   // 设置刻度为0时的网格线的画笔
    customPlot->yAxis->grid()->setPen(QPen(yGridPen, 2, gridPenStyle));
    //！！！y轴网格线可见
    customPlot->yAxis->grid()->setVisible(false);
    customPlot->yAxis->grid()->setSubGridPen(QPen(ySubGridPen, 1, subGridPenStyle));
    customPlot->yAxis->grid()->setSubGridVisible(ySubGridVisible);
    customPlot->yAxis->grid()->setZeroLinePen(QPen(zeroLinePen));


    //加的
    //    customPlot->xAxis->setLabel("time");
    //    customPlot->yAxis->setLabel("values");

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime); //时间日期作为X轴
    timeTicker->setDateTimeFormat("ss");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->xAxis->setRange(0, this->xAxisUpper);

    customPlot->axisRect()->setupFullAxesBox();
    //移除右上方曲线
    customPlot->axisRect()->removeAxis(customPlot->axisRect()->axis(QCPAxis::atRight));
    customPlot->axisRect()->removeAxis(customPlot->axisRect()->axis(QCPAxis::atTop));

    //设置y轴范围！！
    customPlot->yAxis->setRange(0, this->yAxisUpper);

    //设置字体
    customPlot->setFont(QFont(font().family(), labelSize));

    //曲线名称可见
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica",9));
    //设置曲线名称大小
    customPlot->legend->setIconSize(0,0);


    customPlot->setVisible(true);     //设置层是否可见

    //    for (int i = 0; i < linesNum; ++i)
    //    {
    //        customPlot->graph(i)->setSelectable(QCP::SelectionType::stMultipleDataRanges);
    //    }

    //选择框模式：无
    customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
    //选框黑色虚线
    customPlot->selectionRect()->setPen(QPen(Qt::black,1,Qt::DashLine));
    customPlot->selectionRect()->setBrush(QBrush(QColor(0,0,100,50)));
    //修改多选按键，默认Ctrl
    customPlot->setMultiSelectModifier(Qt::KeyboardModifier::NoModifier);
    customPlot->setInteractions(QCP::iSelectPlottables| QCP::iMultiSelect);

    connect(myCustomPlot, &QCustomPlot::sendRectCustom, this, &DynamicPlot::magnify);
    connect(myCustomPlot, &QCustomPlot::mousePress, this, &DynamicPlot::showTracer);
    //刷新

    //x坐标轴可见加上的话曲线显示区域太小
    customPlot->xAxis->setVisible(false);
    customPlot->yAxis->setVisible(false);
    //setXYvisbile(false);
    customPlot->replot(QCustomPlot::rpQueuedReplot);

}
//设置xy坐标轴是否可见
void DynamicPlot::setXYvisbile(bool f)
{
    this->myCustomPlot->xAxis->setVisible(f);
    this->myCustomPlot->yAxis->setVisible(f);

    this->myCustomPlot->xAxis->setLabelColor(QColor(255, 0, 0));
    this->myCustomPlot->xAxis->setTickLabelColor(QColor(255, 0, 0));
    this->myCustomPlot->yAxis->setLabelColor(QColor(255, 0, 0));
    this->myCustomPlot->yAxis->setTickLabelColor(QColor(255, 0, 0));
    this->myCustomPlot->xAxis->setTickLabelColor(QColor(255, 0, 0));           //轴刻度线文字颜色
    this->myCustomPlot->yAxis->setTickLabelColor(QColor(255, 0, 0));
    this->myCustomPlot->xAxis->setLabelColor(labelPen);
    this->myCustomPlot->yAxis->setLabelColor(labelPen);
    this->myCustomPlot->xAxis->grid()->setVisible(f);
    this->myCustomPlot->yAxis->grid()->setVisible(f);
    this->myCustomPlot->xAxis->setLabel("time");
    this->myCustomPlot->yAxis->setLabel("values");

    this->myCustomPlot->replot();

}
//-----------------------------------------------------

DynamicPlot::~DynamicPlot()
{
    delete ui;

    if (myCustomPlot != nullptr)
    {
        delete myCustomPlot;
    }
    if (propertyBrowser != nullptr)
    {
        delete propertyBrowser;
    }
    if (toolBar != nullptr)
    {
        delete toolBar;
    }
    if (vLayoutToolBar != nullptr)
    {
        delete vLayoutToolBar;
    }
    if (sideToolBar != nullptr)
    {
        delete sideToolBar;
    }
    if (m_TracerX)
    {
        m_TracerX.clear();
    }
    for (int i = 0; i < m_TracersY.length(); ++i)
    {
        m_TracersY[i].clear();
    }
    if (tableAnimation != nullptr)
    {
        delete tableAnimation;
    }
    if (plotAnimation != nullptr)
    {
        delete plotAnimation;
    }
}

void DynamicPlot::setToolBar()
{
    sideToolBar = new QWidget();
    sideToolBar->setObjectName("sideToolBar");
    sideToolBar->setStyleSheet(QString::fromUtf8("#sideToolBar{border:1px solid #a2a2a2}"));
    sideToolBar->setFixedWidth(30);
    //    menuDialog->setWindowFlags(Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    toolBar = new QToolBar("");
    toolBar->addAction(ui->action_open);
    toolBar->addAction(ui->action_save);
    toolBar->addAction(ui->action_save_as);
    toolBar->addAction(ui->action_hide);
    toolBar->addAction(ui->action_unfold);
    toolBar->addAction(ui->action_magnify);
    toolBar->addAction(ui->action_new_win_magnify);
    toolBar->addAction(ui->action_recover);
    toolBar->setMovable(false);
    toolBar->setOrientation(Qt::Horizontal);

    toolBar->setFloatable(false);//设置是否能够浮动
    toolBar->setOrientation(Qt::Vertical);
    toolBar->setAllowedAreas(Qt::LeftToolBarArea);//设置停靠区域
    vLayoutToolBar = new QHBoxLayout();
    vLayoutToolBar->setContentsMargins(1,0,1,1);
    vLayoutToolBar->addWidget(toolBar);
    vLayoutToolBar->setSpacing(0);
    sideToolBar->setLayout(vLayoutToolBar);
    layout()->addWidget(sideToolBar);
}

void DynamicPlot::readData()
{
    //    fileName = QFileDialog::getOpenFileName(this, tr("选择数据文件"), "", tr("TEXT(*.txt);;DAT(*.dat)"));
    if (fileName.isEmpty())
    {
        return;
    }
    QFile file(fileName);

    QVector<QCPGraphData> temp;
    QRegExp Separator = QRegExp("\t|\n|\r\n|\r| |,");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QStringList oneLine = in.readLine().split(Separator, QString::SkipEmptyParts);
            QListIterator<QString> list(oneLine);
            while (list.hasNext())
            {
                if (linesNum == 0)
                {
                    linesNum = list.next().toInt();
                    ++curLineIndex;
                    if (list.hasNext())
                    {
                        file.close();
                        return;
                    }
                    break;
                }
                if (QString::compare(list.peekNext(), ";") == 0)
                {
                    data.append(temp);
                    temp.clear();
                    ++curLineIndex;
                    break;
                }
                QCPGraphData newPoint;

                newPoint.key = list.next().toDouble();
                if (!list.hasNext())
                {
                    file.close();
                    data.clear();
                    return;
                }
                newPoint.value = list.next().toDouble();
                temp.append(newPoint);
            }

        }
    }
    file.close();
    for (int i = 0; i < linesNum; ++i)
    {
        std::sort(data[i].begin(), data[i].end(), comQtGraph);
    }

}



void DynamicPlot::init()
{
    linesName.resize(linesNum);
    linesPen.resize(linesNum);
    linesColor.resize(linesNum);
    linePenStyle.resize(linesNum);
    lineStyle.resize(linesNum);
    lineScatterStyle.resize(linesNum);
    lineVisible.resize(linesNum);
    linesWidth.resize(linesNum);
    for (int i = 0; i < linesNum; ++i)
    {
        curveProperties.append(QVector<double>(5, 0));
        curvePropertiesFM.append(QVector<double>(5, 0));
        curvePropertiesTemp.append(QVector<double>(5, 0));
    }
    idToProperty.resize(linesNum);
    myGraph.resize(linesNum);
    styleFileVarForLines.resize(linesNum);
    enumManagerLines.resize(linesNum);
}


//读样式文件
void DynamicPlot::readPropertiesFile(const QString fName)
{
    if (fName.isEmpty())
    {
        return;
    }
    QFile file(fName);
    QRegExp Separator = QRegExp("\t|\n|\r\n|\r| |,|:");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        in.setCodec("System");
        bool tag = true;
        bool isLinesVariable = false;
        int index = -1;
        while (tag && !in.atEnd())
        {
            QStringList oneLine = in.readLine().split(Separator, QString::SkipEmptyParts);
            QListIterator<QString> list(oneLine);
            while (list.hasNext())
            {
                QString first = list.next();
                if (list.hasNext())
                {
                    if (!QString::compare(first, "曲线条数"))
                    {
                        if (list.next().toInt() != linesNum)
                        {
                            tag = false;
                            break;
                        }
                        continue;
                    }
                    else if (!QString::compare(first, "曲线序号"))
                    {
                        isLinesVariable = true;
                        index = list.next().toInt();
                        continue;
                    }

                    QVariant var;
                    var.setValue(list.next());
                    if (!isLinesVariable)
                    {
                        switch (styleFileVariables[first].style)
                        {
                        case VARIABLE_STYLE::STYLE_INT:
                            *(static_cast<int*>(styleFileVariables[first].vPtr.value<void*>())) = var.toInt();
                            break;
                        case VARIABLE_STYLE::STYLE_BOOL:
                            *(static_cast<bool*>(styleFileVariables[first].vPtr.value<void*>())) = var.toBool();
                            break;
                        case VARIABLE_STYLE::STYLE_COLOR:
                            *(static_cast<QColor*>(styleFileVariables[first].vPtr.value<void*>())) = stringToColor(var.toString());
                            break;
                        case VARIABLE_STYLE::STYLE_DOUBLE:
                            *(static_cast<double*>(styleFileVariables[first].vPtr.value<void*>())) = var.toDouble();
                            break;
                        case VARIABLE_STYLE::STYLE_STRING:
                            *(static_cast<QString*>(styleFileVariables[first].vPtr.value<void*>())) = var.toString();
                            break;
                        case VARIABLE_STYLE::STYLE_PEN_STYLE:
                            *(static_cast<int*>(styleFileVariables[first].vPtr.value<void*>())) = Qt::PenStyle(var.toInt());
                            break;
                        }
                    }
                    else
                    {
                        switch (styleFileVarForLines[index][first].style)
                        {
                        case VARIABLE_STYLE::STYLE_INT:
                            *(static_cast<int*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = var.toInt();
                            break;
                        case VARIABLE_STYLE::STYLE_BOOL:
                            *(static_cast<bool*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = var.toBool();
                            break;
                        case VARIABLE_STYLE::STYLE_COLOR:
                            *(static_cast<QColor*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = stringToColor(var.toString());
                            break;
                        case VARIABLE_STYLE::STYLE_STRING:
                            *(static_cast<QString*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = var.toString();
                            break;
                        case VARIABLE_STYLE::STYLE_PEN_STYLE:
                            *(static_cast<int*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = Qt::PenStyle(var.toInt());
                            break;
                        case VARIABLE_STYLE::STYLE_LINE_STYLE:
                            *(static_cast<int*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = LINE_STYLE(var.toInt());
                            break;
                        case VARIABLE_STYLE::STYLE_SCATTER_STYLE:
                            *(static_cast<int*>(styleFileVarForLines[index][first].vPtr.value<void*>())) = SCATTER_SHAPE(var.toInt());
                            break;
                        }
                    }
                }

            }
        }

    }
    file.close();
    for (int i = 0; i <= int(AXIS_EPROPERTIES_NAME::LABEL_SIZE); ++i)
    {
        updateAxisProperties(AXIS_EPROPERTIES_NAME(i));
    }
    for (int j = 0; j < linesNum; ++j)
    {
        curLineIndex = j;
        for (int i = 0; i <= int(LINE_EPROPERTIES_NAME::LINE_WIDTH); ++i)
        {
            updateLineProperties(LINE_EPROPERTIES_NAME(i));
        }
        for (int i = 0; i <= int(ENUM_EPROPERTIES_NAME::LINE_SCATTER_STYLE); ++i)
        {
            updateEnumProperties(ENUM_EPROPERTIES_NAME(i));
        }
    }
    QMapIterator<QtVariantProperty*, AxisPropertyIndex> axisIterator(axisVarProperties);
    while (axisIterator.hasNext())
    {
        axisIterator.next();
        QString name = axisIterator.value().variableName;
        QtVariantProperty* varProperty = axisIterator.key();
        switch (styleFileVariables[name].style)
        {
        case VARIABLE_STYLE::STYLE_INT:
            varProperty->setValue(*(static_cast<int*>(styleFileVariables[name].vPtr.value<void*>())));
            break;
        case VARIABLE_STYLE::STYLE_BOOL:
            varProperty->setValue(*(static_cast<bool*>(styleFileVariables[name].vPtr.value<void*>())));
            break;
        case VARIABLE_STYLE::STYLE_COLOR:
            varProperty->setValue(QColor(*(static_cast<QColor*>(styleFileVariables[name].vPtr.value<void*>()))));
            break;
        case VARIABLE_STYLE::STYLE_DOUBLE:
            varProperty->setValue(*(static_cast<double*>(styleFileVariables[name].vPtr.value<void*>())));
            break;
        case VARIABLE_STYLE::STYLE_STRING:
            varProperty->setValue(*(static_cast<QString*>(styleFileVariables[name].vPtr.value<void*>())));
            break;
        }
    }
    QMapIterator<QtProperty*, EnumPropertyIndex> enumIterator(enumProperties);
    while (enumIterator.hasNext())
    {
        enumIterator.next();
        QString name = enumIterator.value().variableName;
        int index = enumIterator.value().second;
        if ( index== -1 && enumManagerAxis->hasProperty(enumIterator.key()))
        {
            enumManagerAxis->setValue(enumIterator.key(), *(static_cast<int*>(styleFileVariables[name].vPtr.value<void*>())));
        }
        else if (enumManagerLines[index]->hasProperty(enumIterator.key()))
        {
            enumManagerLines[index]->setValue(enumIterator.key(), *(static_cast<int*>(styleFileVarForLines[index][name].vPtr.value<void*>())));
        }
    }
    QMapIterator<QtVariantProperty*, LinePropertyIndex> lineIterator(linesVarProperties);
    while (lineIterator.hasNext())
    {
        lineIterator.next();
        QString name = lineIterator.value().variableName;
        int index = lineIterator.value().second;
        QtVariantProperty* varProperty = lineIterator.key();
        switch (styleFileVarForLines[index][name].style)
        {
        case VARIABLE_STYLE::STYLE_INT:
            varProperty->setValue(*(static_cast<int*>(styleFileVarForLines[index][name].vPtr.value<void*>())));
            break;
        case VARIABLE_STYLE::STYLE_BOOL:
            varProperty->setValue(*(static_cast<bool*>(styleFileVarForLines[index][name].vPtr.value<void*>())));
            break;
        case VARIABLE_STYLE::STYLE_COLOR:
            varProperty->setValue(QColor(*(static_cast<QColor*>(styleFileVarForLines[index][name].vPtr.value<void*>()))));
            break;
        case VARIABLE_STYLE::STYLE_STRING:
            varProperty->setValue(*(static_cast<QString*>(styleFileVarForLines[index][name].vPtr.value<void*>())));
            break;
        }
    }
}



//保存配置文件
void DynamicPlot::writePropertiesFile(QString fName)
{
    if (fName.isEmpty())
    {
        return;
    }
    QFile file(fName);
    if (file.open(QIODevice::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&file);
        out.setCodec("System");    //输出编码设为System
        QMapIterator<QString, T> iterator(styleFileVariables);
        while (iterator.hasNext())
        {
            iterator.next();
            out << iterator.key() << ":\t";
            switch (iterator.value().style)
            {
            case VARIABLE_STYLE::STYLE_INT:
                out << *(static_cast<int*>(iterator.value().vPtr.value<void*>())) << endl;
                break;
            case VARIABLE_STYLE::STYLE_BOOL:
                out << *(static_cast<bool*>(iterator.value().vPtr.value<void*>())) << endl;
                break;
            case VARIABLE_STYLE::STYLE_COLOR:
                out << colorToString(*(static_cast<QColor*>(iterator.value().vPtr.value<void*>()))) << endl;
                break;
            case VARIABLE_STYLE::STYLE_DOUBLE:
                out << *(static_cast<double*>(iterator.value().vPtr.value<void*>())) << endl;
                break;
            case VARIABLE_STYLE::STYLE_STRING:
                out << *(static_cast<QString*>(iterator.value().vPtr.value<void*>())) << endl;
                break;
            case VARIABLE_STYLE::STYLE_PEN_STYLE:
                out << *(static_cast<int*>(iterator.value().vPtr.value<void*>())) << endl;
                break;
            }
        }
        out << endl;
        for (int i = 0; i < linesNum; ++i)
        {
            out << tr("曲线序号") << ":\t" << i << endl;
            QMapIterator<QString, T> iterator(styleFileVarForLines[i]);
            while (iterator.hasNext())
            {
                iterator.next();
                out << iterator.key() << ":\t";
                switch (iterator.value().style)
                {
                case VARIABLE_STYLE::STYLE_INT:
                    out << *(static_cast<int*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_BOOL:
                    out << *(static_cast<bool*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_COLOR:
                    out << colorToString(*(static_cast<QColor*>(iterator.value().vPtr.value<void*>()))) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_DOUBLE:
                    out << *(static_cast<double*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_STRING:
                    out << *(static_cast<QString*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_PEN_STYLE:
                    out << *(static_cast<int*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_LINE_STYLE:
                    out << *(static_cast<int*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                case VARIABLE_STYLE::STYLE_SCATTER_STYLE:
                    out << *(static_cast<int*>(iterator.value().vPtr.value<void*>())) << endl;
                    break;
                }
            }
            out << endl;
        }

    }
    file.close();
}

void DynamicPlot::calculatesPropertyValue(const QVector<QVector<QCPGraphData>>& vector, QVector<QVector<double>>& properties)
{
    for (int i = 0; i < properties.length(); ++i)
    {
        properties[i].resize(5);
    }
    int length = vector.size();
    properties.resize(length);
    QVector<double> sum(length, 0);
    QVector<double> sumOfSquares(length, 0);
    QVector<double> count(length, 0);
    for (int i = 0; i < length; ++i)
    {
        properties[i][0] = INT_MIN;
        properties[i][1] = INT_MAX;
        for (int j = 0; j < vector[i].length(); ++j)
        {
            sum[i] += vector[i][j].value;
            sumOfSquares[i] += qPow(vector[i][j].value, 2);
            properties[i][0] = properties[i][0] > vector[i][j].value ? properties[i][0] : vector[i][j].value;
            properties[i][1] = properties[i][1] < vector[i][j].value ? properties[i][1] : vector[i][j].value;
            ++count[i];
        }
        if (qFuzzyIsNull(count[i]))
        {
            properties[i][2] = INT_MIN;
            properties[i][3] = INT_MIN;
            properties[i][4] = INT_MIN;
        }
        else
        {
            properties[i][2] = sum[i] / count[i];
            properties[i][3] = properties[i][0] - properties[i][1];
            properties[i][4] = sqrt(sumOfSquares[i] / count[i]);
        }

    }
}

void DynamicPlot::getXYLowerAndUpper(const QVector<QVector<QCPGraphData> > &vector)
{
    for (int i = 0; i < linesNum; ++i)
    {
        xAxisLower = xAxisLower < vector[i][0].key ? xAxisLower : vector[i][0].key;
        xAxisUpper = xAxisUpper > vector[i][vector[i].length() - 1].key ? xAxisUpper : vector[i][vector[i].length() - 1].key;
        yAxisLower = yAxisLower < curveProperties[i][1] ? yAxisLower : curveProperties[i][1];
        yAxisUpper = yAxisUpper > curveProperties[i][0] ? yAxisUpper : curveProperties[i][0];
    }
}

QString DynamicPlot::colorToString(const QColor color)
{
    QRgb mRgb = qRgb(color.red(), color.green(), color.blue());
    return QString::number(mRgb, 16);
}

QColor DynamicPlot::stringToColor(const QString str)
{
    return QColor(str.toUInt(nullptr, 16));
}



void DynamicPlot::initDrawing()
{
    int r = 255;
    int g = 255;
    int b = 255;

    for (int i = 0; i < linesNum; ++i)
    {
        //添加曲线名称在这
        linesName[i].append(this->line_name);

        switch (i % 3) {
        case 0: r = (r + 255 - 100) % 255;
            break;
        case 1: g = (g + 255 - 100) % 255;
            break;
        case 2: b = (b + 255 - 100) % 255;
            break;
        }
        //设置曲线颜色
        r=0;
        g=0;
        b=255;
        linesColor[i] = QColor(r, g, b);
        linePenStyle[i] = Qt::PenStyle::SolidLine;
        lineStyle[i] = QCPGraph::LineStyle::lsLine;
        lineScatterStyle[i] = QCPScatterStyle::ScatterShape::ssNone;
        linesWidth[i] = 1;
        lineVisible[i] = true;
    }
}



void DynamicPlot::addLineGraph(QCustomPlot* customPlot,  const int index)
{


    myGraph[index] = customPlot->addGraph();
    //    QSharedPointer<QCPGraphDataContainer> dataContainer;
    //    dataContainer = myGraph[index]->data();
    //    dataContainer->set(vector, true);

    //设置线条样式
    linesPen[index].setWidth(linesWidth[index]);     //曲线粗细
    linesPen[index].setColor(linesColor[index]);     //曲线颜色
    linesPen[index].setStyle(linePenStyle[index]);
    myGraph[index]->setPen(linesPen[index]);
    myGraph[index]->setLineStyle(lineStyle[index]);   //设置曲线连接方式
    myGraph[index]->setScatterStyle(QCPScatterStyle(lineScatterStyle[index], scatterSize)); //设置曲线节点样式
    myGraph[index]->setAntialiasedFill(true);  //设置抗锯齿

    //设置曲线名字

    myGraph[index]->setName(linesName[index]);
    if (0 == index)
    {
        m_TracerX = QPointer<MyTracer> (new MyTracer(myCustomPlot, myCustomPlot->graph(0), TracerType::XAxisTracer));
    }
    m_TracersY.append(QPointer<MyTracer> (new MyTracer(myCustomPlot, myCustomPlot->graph(index), TracerType::DataTracer)));
}

//设置属性表
void DynamicPlot::setAxisProperties()
{
    QHBoxLayout *layout = new QHBoxLayout(ui->tableWidget);
    layout->setMargin(0);
    propertyBrowser = new QtTreePropertyBrowser(ui->tableWidget);
    layout->addWidget(propertyBrowser);
    propertyBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);    //设置表头内容自动调整
    QtVariantPropertyManager* pVarManager = new QtVariantPropertyManager(propertyBrowser);
    QtVariantEditorFactory* editFactory = new QtVariantEditorFactory(propertyBrowser);
    propertyBrowser->setFactoryForManager(pVarManager, editFactory);
    enumManagerAxis = new myEnumPropertyManager();
    enumFactoryAxis = new QtEnumEditorFactory();
    propertyBrowser->setFactoryForManager((QtEnumPropertyManager*)enumManagerAxis, enumFactoryAxis);

    QtVariantProperty* propertyTitle = pVarManager->addProperty(QVariant::String, QStringLiteral("窗口标题"));
    propertyTitle->setValue(fileName);
    AxisPropertyIndex axisProIndex;
    axisProIndex.first = AXIS_EPROPERTIES_NAME::FILE_NAME;
    axisProIndex.variableName = propertyTitle->propertyName();
    axisProperties[propertyTitle] = axisProIndex;
    axisVarProperties[propertyTitle] = axisProIndex;
    //    qDebug() << propertyTitle->propertyName();
    propertyBrowser->addProperty(propertyTitle);

    QtProperty* propertyBar = pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("背景颜色(渐变)"));
    QtVariantProperty* item = pVarManager->addProperty(QVariant::Color, QStringLiteral("背景颜色(上)"));
    item->setValue(background0);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::BACKGROUND_0;
    axisProIndex.variableName = item->propertyName();
    axisProperties[item] = axisProIndex;
    axisVarProperties[item] = axisProIndex;
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Color, QStringLiteral("背景颜色(下)"));
    item->setValue(background1);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::BACKGROUND_1;
    axisProIndex.variableName = item->propertyName();
    axisProperties[item] = axisProIndex;
    axisVarProperties[item] = axisProIndex;
    propertyBar->addSubProperty(item);

    propertyBrowser->addProperty(propertyBar);

    //-----------坐标轴设置--------------------------------------------------
    QtProperty* propertyBar00 = pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("坐标轴设置"));
    QtVariantProperty* item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("x坐标轴颜色"));
    item00->setValue(xBasePen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_BASE_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("y坐标轴颜色"));
    item00->setValue(yBasePen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_BASE_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Int, QStringLiteral("坐标轴宽度"));
    item00->setValue(basePenWidth);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::BASE_PEN_WIDTH;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("刻度线颜色"));
    item00->setValue(tickPen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::TICK_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("字体颜色"));
    item00->setValue(labelPen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::LABEL_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Int, QStringLiteral("字体大小"));
    item00->setValue(labelSize);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::LABEL_SIZE;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("x轴网格线颜色"));
    item00->setValue(xGridPen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_GRID_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("y轴网格线颜色"));
    item00->setValue(yGridPen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_GRID_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Bool, QStringLiteral("子网格线x可见"));
    item00->setValue(xSubGridVisible);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_SUBGRID_VISIBLE;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Bool, QStringLiteral("子网格线y可见"));
    item00->setValue(ySubGridVisible);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_SUBGRID_VISIBLE;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("x轴子网格线颜色"));
    item00->setValue(xSubGridPen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_SUBGRID_PEN;
    axisProIndex.variableName = item00->propertyName();
    propertyBar00->addSubProperty(item00);
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;

    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("y轴子网格线颜色"));
    item00->setValue(ySubGridPen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_SUBGRID_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    //---------------------------------------------------------------------------
    QStringList linePenStyles;
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
        linePenStyles << metaEnum.key(i);
    }
    QtProperty* gridPenStylePro = enumManagerAxis->addProperty("网格线样式");
    enumManagerAxis->setEnumNames(gridPenStylePro, linePenStyles);
    enumManagerAxis->setValue(gridPenStylePro, 3);
    EnumPropertyIndex enumIndex;
    enumIndex.first = ENUM_EPROPERTIES_NAME::GRID_PEN_STYLE;
    enumIndex.second = -1;
    enumIndex.variableName = gridPenStylePro->propertyName();
    enumProperties[gridPenStylePro] = enumIndex;
    propertyBar00->addSubProperty(gridPenStylePro);

    QtProperty* subGridPenStylePro = enumManagerAxis->addProperty("子网格线样式");
    enumManagerAxis->setEnumNames(subGridPenStylePro, linePenStyles);
    enumManagerAxis->setValue(subGridPenStylePro, 3);
    enumIndex.first = ENUM_EPROPERTIES_NAME::SUBGRID_PEN_STYLE;
    enumIndex.second = -1;
    enumIndex.variableName = subGridPenStylePro->propertyName();
    enumProperties[subGridPenStylePro] = enumIndex;
    propertyBar00->addSubProperty(subGridPenStylePro);

    connect(enumManagerAxis, &myEnumPropertyManager::valueChanged, this, &DynamicPlot::enumValueChanged);
    //---------------------------------------------------------------------------
    item00 = pVarManager->addProperty(QVariant::Color, QStringLiteral("网格线0颜色"));
    item00->setValue(zeroLinePen);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::ZERO_LINE_PEN;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::String, QStringLiteral("x轴名称"));
    item00->setValue(xAxisName);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_AXIS_NAME;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::String, QStringLiteral("单位x"));
    item00->setValue(xAxisUnit);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_AXIS_UNIT;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::String, QStringLiteral("y轴名称"));
    item00->setValue(yAxisName);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_AXIS_NAME;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::String, QStringLiteral("单位y"));
    item00->setValue(yAxisUnit);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_AXIS_UNIT;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Double, QStringLiteral("x轴上限"));
    item00->setValue(xAxisUpper);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_AXIS_UPPER;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Double, QStringLiteral("x轴下限"));
    item00->setValue(xAxisLower);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::X_AXIS_LOWER;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Double, QStringLiteral("y轴上限"));
    item00->setValue(yAxisUpper);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_AXIS_UPPER;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    item00 = pVarManager->addProperty(QVariant::Double, QStringLiteral("y轴下限"));
    item00->setValue(yAxisLower);
    axisProIndex.first = AXIS_EPROPERTIES_NAME::Y_AXIS_LOWER;
    axisProIndex.variableName = item00->propertyName();
    axisProperties[item00] = axisProIndex;
    axisVarProperties[item00] = axisProIndex;
    propertyBar00->addSubProperty(item00);

    propertyBrowser->addProperty(propertyBar00);
    //-----------坐标轴设置--------------------------------------------------
    connect(pVarManager, &QtVariantPropertyManager::valueChanged, this, &DynamicPlot::axisValueChanged);

    for (int i = 0; i < linesNum; ++i)
    {
        //setLineProperties(i);
    }
}

void DynamicPlot::setLineProperties(const int index)
{
    QtVariantPropertyManager* pVarManager = new QtVariantPropertyManager(propertyBrowser);
    QtVariantEditorFactory* editFactory = new QtVariantEditorFactory(propertyBrowser);
    propertyBrowser->setFactoryForManager(pVarManager, editFactory);
    enumManagerLines[index] = new myEnumPropertyManager();
    enumFactoryLine = new QtEnumEditorFactory();
    propertyBrowser->setFactoryForManager((QtEnumPropertyManager*)enumManagerLines[index], enumFactoryLine);

    //-----------曲线设置----------------------------------------------------
    QString tempName = "曲线设置";
    tempName.append(QString::number(index));
    QtProperty* propertyBar = pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), tempName.toUtf8());
    QtVariantProperty* item = pVarManager->addProperty(QVariant::String, QStringLiteral("曲线名称"));
    item->setValue(linesName[index]);

    LinePropertyIndex lineIndex;
    lineIndex.first = LINE_EPROPERTIES_NAME::LINES_NAME;
    lineIndex.second = index;
    lineIndex.variableName = item->propertyName();
    linesProperties[item] = lineIndex;
    linesVarProperties[item] = lineIndex;
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Bool, QStringLiteral("曲线可见"));
    item->setValue(lineVisible[index]);
    lineIndex.first = LINE_EPROPERTIES_NAME::LINE_VISIBLE;
    lineIndex.second = index;
    lineIndex.variableName = item->propertyName();
    linesProperties[item] = lineIndex;
    linesVarProperties[item] = lineIndex;
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Color, QStringLiteral("曲线颜色"));
    item->setValue(linesColor[index]);
    lineIndex.first = LINE_EPROPERTIES_NAME::LINES_COLOR;
    lineIndex.second = index;
    lineIndex.variableName = item->propertyName();
    linesProperties[item] = lineIndex;
    linesVarProperties[item] = lineIndex;
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Int, QStringLiteral("曲线宽度"));
    item->setValue(linesWidth[index]);
    //qDebug()<<"曲线宽带debug :"<<linesWidth[index];
    lineIndex.first = LINE_EPROPERTIES_NAME::LINE_WIDTH;
    lineIndex.second = index;
    lineIndex.variableName = item->propertyName();
    linesProperties[item] = lineIndex;
    linesVarProperties[item] = lineIndex;
    propertyBar->addSubProperty(item);

    //-------------------------------------------------------------
    QStringList linePenStyles;
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::PenStyle>();
    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
        linePenStyles << metaEnum.key(i);
    }
    QtProperty* penStyle = enumManagerLines[index]->addProperty("曲线样式");
    enumManagerLines[index]->setEnumNames(penStyle, linePenStyles);
    enumManagerLines[index]->setValue(penStyle, 1);
    EnumPropertyIndex enumIndex;
    enumIndex.first = ENUM_EPROPERTIES_NAME::LINE_PEN_STYLE;
    enumIndex.second = index;
    enumIndex.variableName = penStyle->propertyName();
    enumProperties[penStyle] = enumIndex;
    propertyBar->addSubProperty(penStyle);

    QStringList lineStyles;
    metaEnum = QMetaEnum::fromType<DynamicPlot::LINE_STYLE>();
    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
        lineStyles << metaEnum.key(i);
    }
    QtProperty* lStyle = enumManagerLines[index]->addProperty("曲线连接方式");
    enumManagerLines[index]->setEnumNames(lStyle, lineStyles);
    enumManagerLines[index]->setValue(lStyle, 1);
    enumIndex.first = ENUM_EPROPERTIES_NAME::LINE_STYLE;
    enumIndex.second = index;
    enumIndex.variableName = lStyle->propertyName();
    enumProperties[lStyle] = enumIndex;
    propertyBar->addSubProperty(lStyle);

    QStringList lineScatterStyles;
    metaEnum = QMetaEnum::fromType<DynamicPlot::SCATTER_SHAPE>();
    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
        lineScatterStyles << metaEnum.key(i);
    }
    QtProperty* scatterStyle = enumManagerLines[index]->addProperty("曲线节点样式");
    scatterStyle->setStatusTip(QString(index));
    enumManagerLines[index]->setEnumNames(scatterStyle, lineScatterStyles);
    enumManagerLines[index]->setValue(scatterStyle, 0);
    enumIndex.first = ENUM_EPROPERTIES_NAME::LINE_SCATTER_STYLE;
    enumIndex.second = index;
    enumIndex.variableName = scatterStyle->propertyName();
    enumProperties[scatterStyle] = enumIndex;
    propertyBar->addSubProperty(scatterStyle);

    connect(enumManagerLines[index], &myEnumPropertyManager::valueChanged, this, &DynamicPlot::enumValueChanged);
    //-------------------------------------------------------------
    //---------------------------------------------------------------------


    //不可更改的属性，最大最小值、PV/RMS、平均值；
    QtVariantPropertyManager* pVarManager01 = new QtVariantPropertyManager(propertyBrowser);
    //
    QtProperty* curveProperty = pVarManager01->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("属性值"));
    QtVariantProperty* item01 = pVarManager01->addProperty(QVariant::Double, QStringLiteral("最大值"));
    item01->setValue(curveProperties[index][0]);
    idToProperty[index][EPROPERTY_VALUE::MAXIMUM] = item01;
    curveProperty->addSubProperty(item01);

    item01 = pVarManager01->addProperty(QVariant::Double, QStringLiteral("最小值"));
    item01->setValue(curveProperties[index][1]);
    idToProperty[index][EPROPERTY_VALUE::MINIMUM] = item01;
    curveProperty->addSubProperty(item01);

    item01 = pVarManager01->addProperty(QVariant::Double, QStringLiteral("平均值"));
    item01->setValue(curveProperties[index][2]);
    idToProperty[index][EPROPERTY_VALUE::AVERAGE] = item01;
    curveProperty->addSubProperty(item01);

    item01 = pVarManager01->addProperty(QVariant::Double, QStringLiteral("PV值"));
    item01->setValue(curveProperties[index][3]);
    idToProperty[index][EPROPERTY_VALUE::PV] = item01;
    curveProperty->addSubProperty(item01);

    item01 = pVarManager01->addProperty(QVariant::Double, QStringLiteral("RMS值"));
    item01->setValue(curveProperties[index][4]);
    idToProperty[index][EPROPERTY_VALUE::RMS] = item01;
    curveProperty->addSubProperty(item01);

    propertyBar->addSubProperty(curveProperty);
    propertyBrowser->addProperty(propertyBar);
    connect(pVarManager, &QtVariantPropertyManager::valueChanged, this, &DynamicPlot::lineValueChanged);
}

void DynamicPlot::setCurveProperties(const QVector<QVector<double>> &vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        idToProperty[i][EPROPERTY_VALUE::MAXIMUM]->setValue(vec[i][0]);
        idToProperty[i][EPROPERTY_VALUE::MINIMUM]->setValue(vec[i][1]);
        idToProperty[i][EPROPERTY_VALUE::AVERAGE]->setValue(vec[i][2]);
        idToProperty[i][EPROPERTY_VALUE::PV]->setValue(vec[i][3]);
        idToProperty[i][EPROPERTY_VALUE::RMS]->setValue(vec[i][4]);
    }
}

void DynamicPlot::axisValueChanged(QtProperty *property, const QVariant &value)
{

    AXIS_EPROPERTIES_NAME propertyName = axisProperties[property].first;
    switch (propertyName)
    {
    case AXIS_EPROPERTIES_NAME::FILE_NAME:
        fileName = value.value<QString>();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::BACKGROUND_0:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            background0 = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::BACKGROUND_1:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            background1 = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::X_GRID_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            xGridPen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::Y_GRID_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            yGridPen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::X_SUBGRID_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            xSubGridPen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::Y_SUBGRID_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            ySubGridPen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_NAME:
        xAxisName = value.toString();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_UNIT:
        xAxisUnit = value.toString();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_NAME:
        yAxisName = value.toString();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_UNIT:
        yAxisUnit = value.toString();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_UPPER:
        xAxisUpper = value.toDouble();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_LOWER:
        xAxisLower = value.toDouble();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_UPPER:
        yAxisUpper = value.toDouble();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_LOWER:
        yAxisLower = value.toDouble();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::X_SUBGRID_VISIBLE:
        xSubGridVisible = value.toBool();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::Y_SUBGRID_VISIBLE:
        ySubGridVisible = value.toBool();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::ZERO_LINE_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            zeroLinePen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::X_BASE_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            xBasePen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::Y_BASE_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            yBasePen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::BASE_PEN_WIDTH:
        basePenWidth = value.value<int>();
        updateAxisProperties(propertyName);
        break;
    case AXIS_EPROPERTIES_NAME::TICK_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            tickPen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::LABEL_PEN:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            labelPen = value.value<QColor>();
            updateAxisProperties(propertyName);
        }
        break;
    case AXIS_EPROPERTIES_NAME::LABEL_SIZE:
        labelSize = value.value<int>();
        updateAxisProperties(propertyName);
        break;
    }

}

void DynamicPlot::lineValueChanged(QtProperty *property, const QVariant &value)
{

    if (linesProperties.find(property) == linesProperties.end())
    {
        return;
    }
    LinePropertyIndex lineIndex = linesProperties[property];
    curLineIndex = lineIndex.second;
    switch (lineIndex.first)
    {
    case LINE_EPROPERTIES_NAME::LINE_VISIBLE:
        lineVisible[curLineIndex] = value.toBool();
        updateLineProperties(lineIndex.first);
        break;
    case LINE_EPROPERTIES_NAME::LINES_NAME:
        linesName[curLineIndex] = value.toString();
        updateLineProperties(lineIndex.first);
        break;
    case LINE_EPROPERTIES_NAME::LINES_COLOR:
        if (value.type() == QVariant::Color && value.value<QColor>() != nullptr)
        {
            linesColor[curLineIndex] = value.value<QColor>();
            updateLineProperties(lineIndex.first);
        }
        break;
    case LINE_EPROPERTIES_NAME::LINE_WIDTH:
        linesWidth[curLineIndex] = value.value<int>();
        updateLineProperties(lineIndex.first);
        break;
    }
}

void DynamicPlot::enumValueChanged(QtProperty *property, int val)
{

    EnumPropertyIndex enumIndex = enumProperties[property];
    curLineIndex = enumIndex.second;
    switch (enumIndex.first)
    {
    case ENUM_EPROPERTIES_NAME::LINE_PEN_STYLE:
        linePenStyle[curLineIndex] = Qt::PenStyle(val);
        updateEnumProperties(enumIndex.first);
        break;
    case ENUM_EPROPERTIES_NAME::LINE_STYLE:
        lineStyle[curLineIndex] = QCPGraph::LineStyle(val);
        updateEnumProperties(enumIndex.first);
        break;
    case ENUM_EPROPERTIES_NAME::LINE_SCATTER_STYLE:
        lineScatterStyle[curLineIndex] = QCPScatterStyle::ScatterShape(val);
        updateEnumProperties(enumIndex.first);
        break;
    case ENUM_EPROPERTIES_NAME::GRID_PEN_STYLE:
        gridPenStyle = Qt::PenStyle(val);
        updateEnumProperties(enumIndex.first);
        break;
    case ENUM_EPROPERTIES_NAME::SUBGRID_PEN_STYLE:
        subGridPenStyle = Qt::PenStyle(val);
        updateEnumProperties(enumIndex.first);
        break;
    }
}

void DynamicPlot::setBackgrand()
{
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, background0);
    plotGradient.setColorAt(1, background1);
    myCustomPlot->setBackground(plotGradient);
}

//通过结构体更新属性
void DynamicPlot::update_Axis_line_enum_properties(PLOT_PROPERTY *plot_property)
{
    //updateAxisProperties

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, plot_property->background0);
    plotGradient.setColorAt(1, plot_property->background1);
    myCustomPlot->setBackground(plotGradient);
    myCustomPlot->xAxis->setLabel(plot_property->xAxisName + "(" + plot_property->xAxisUnit + ")");
    myCustomPlot->xAxis->setLabel(plot_property->xAxisName + "(" + plot_property->xAxisUnit + ")");
    myCustomPlot->yAxis->setLabel(plot_property->yAxisName + "(" + plot_property->yAxisUnit + ")");
    myCustomPlot->yAxis->setLabel(plot_property->yAxisName + "(" + plot_property->yAxisUnit + ")");
    myCustomPlot->xAxis->setRange(plot_property->xAxisLower, plot_property->xAxisUpper);
    myCustomPlot->yAxis->setRange(plot_property->yAxisLower, plot_property->yAxisUpper);
    myCustomPlot->xAxis->grid()->setPen(QPen(plot_property->xGridPen, 2, plot_property->gridPenStyle));
    myCustomPlot->yAxis->grid()->setPen(QPen(plot_property->yGridPen, 2, plot_property->gridPenStyle));
    myCustomPlot->xAxis->grid()->setSubGridPen(QPen(plot_property->xSubGridPen, 1, plot_property->subGridPenStyle));
    myCustomPlot->yAxis->grid()->setSubGridPen(QPen(plot_property->ySubGridPen, 1, plot_property->subGridPenStyle));
    myCustomPlot->xAxis->grid()->setZeroLinePen(QPen(plot_property->zeroLinePen));
    myCustomPlot->yAxis->grid()->setZeroLinePen(QPen(plot_property->zeroLinePen));
    myCustomPlot->xAxis->setBasePen(QPen(plot_property->xBasePen, plot_property->basePenWidth));
    myCustomPlot->yAxis->setBasePen(QPen(plot_property->yBasePen, plot_property->basePenWidth));
    myCustomPlot->xAxis->setTickPen(QPen(plot_property->tickPen, 2));
    myCustomPlot->xAxis->setSubTickPen(QPen(plot_property->tickPen, 1));
    myCustomPlot->yAxis->setTickPen(QPen(plot_property->tickPen, 2));
    myCustomPlot->yAxis->setSubTickPen(QPen(plot_property->tickPen, 1));
    myCustomPlot->xAxis->setTickLabelColor(plot_property->labelPen);
    myCustomPlot->xAxis->setLabelColor(plot_property->labelPen);
    myCustomPlot->yAxis->setTickLabelColor(plot_property->labelPen);
    myCustomPlot->yAxis->setLabelColor(plot_property->labelPen);
    myCustomPlot->xAxis->setLabelFont(QFont(font().family(), plot_property->labelSize));
    myCustomPlot->yAxis->setLabelFont(QFont(font().family(), plot_property->labelSize));
    myCustomPlot->xAxis->grid()->setSubGridVisible(plot_property->xSubGridVisible);
    myCustomPlot->yAxis->grid()->setSubGridVisible(plot_property->ySubGridVisible);

    //updateLineProperties
    myGraph[0]->setVisible(plot_property->lineVisible);

    myGraph[0]->setName(plot_property->linesName);

    linesPen[0].setWidth(plot_property->linesWidth);//曲线粗细
    //结构体中没有pen成员这没改动，可能存在问题
    //myGraph[0]->setPen(linesPen[0]);
    linesPen[0].setColor(plot_property->linesColor);     //曲线颜色
    //myGraph[0]->setPen(linesPen[0]);

    //updateEnumProperties
    linesPen[0].setStyle(plot_property->linePenStyle);
    myGraph[0]->setPen(linesPen[0]);

    myGraph[0]->setLineStyle(plot_property->lineStyle);
    myGraph[0]->setScatterStyle(QCPScatterStyle(plot_property->lineScatterStyle ,scatterSize));
    myCustomPlot->xAxis->grid()->setPen(QPen(plot_property->xGridPen, 2, plot_property->gridPenStyle));
    myCustomPlot->yAxis->grid()->setPen(QPen(yGridPen, 2, gridPenStyle));
    myCustomPlot->xAxis->grid()->setSubGridPen(QPen(plot_property->xSubGridPen, 1, plot_property->subGridPenStyle));
    myCustomPlot->yAxis->grid()->setSubGridPen(QPen(plot_property->ySubGridPen, 1, plot_property->subGridPenStyle));

    //myCustomPlot->replot(QCustomPlot::rpQueuedReplot);

    //用结构体的属性值更新绘图之后必须还要更新右侧属性栏里面的内容不然属性栏还是初始的值
    //功能和dynamicplot中成员函数 update_struct_property反着的
    this->background0 = plot_property->background0;
    this->background1 = plot_property->background1;
    this->xBasePen = plot_property->xBasePen;
    this->yBasePen = plot_property->yBasePen;
    this->basePenWidth=plot_property->basePenWidth;
    this->tickPen=plot_property->tickPen;
    this->labelPen=plot_property->labelPen;
    labelSize=plot_property->labelSize;
    xGridPen=plot_property->xGridPen;
    yGridPen=plot_property->yGridPen;
    xSubGridPen=plot_property->xSubGridPen;
    xSubGridPen=plot_property->ySubGridPen;
    xSubGridPen=plot_property->xSubGridVisible;
    xSubGridPen=plot_property->ySubGridVisible;
    gridPenStyle=plot_property->gridPenStyle;
    subGridPenStyle=plot_property->subGridPenStyle;
    zeroLinePen =plot_property->zeroLinePen;
    xAxisName= plot_property->xAxisName;
    yAxisName= plot_property->yAxisName;
    xAxisUnit = plot_property->xAxisUnit;
    yAxisUnit= plot_property->yAxisUnit;
    xAxisUpper= plot_property->xAxisUpper;
    yAxisUpper =plot_property->yAxisUpper;
    xAxisLower =plot_property->xAxisLower;
    yAxisLower =plot_property->yAxisLower;
    //p->linesName=this->line_name;
    linesName[0]= plot_property->linesName ;
    lineVisible[0]= plot_property->lineVisible;
    linesColor[0] =plot_property->linesColor;
    linesWidth[0] =plot_property->linesWidth;
    linePenStyle[0]=plot_property->linePenStyle;
    lineStyle[0] =plot_property->lineStyle;
    lineScatterStyle[0]= plot_property->lineScatterStyle;
    //让config文件中的属性同步到头文件代码中再调用更新属性让右侧属性更新成文件中的属性值
    this->setAxisProperties();
    this->setLineProperties(0);






}
void DynamicPlot::updateAxisProperties(const AXIS_EPROPERTIES_NAME propertyName)
{
    switch (propertyName) {
    case AXIS_EPROPERTIES_NAME::FILE_NAME:
        emit fileNameSignal(fileName);
        break;
    case AXIS_EPROPERTIES_NAME::BACKGROUND_0:
        setBackgrand();
        break;
    case AXIS_EPROPERTIES_NAME::BACKGROUND_1:
        setBackgrand();
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_NAME:
        myCustomPlot->xAxis->setLabel(xAxisName + "(" + xAxisUnit + ")");
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_UNIT:
        myCustomPlot->xAxis->setLabel(xAxisName + "(" + xAxisUnit + ")");
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_NAME:
        myCustomPlot->yAxis->setLabel(yAxisName + "(" + yAxisUnit + ")");
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_UNIT:
        myCustomPlot->yAxis->setLabel(yAxisName + "(" + yAxisUnit + ")");
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_UPPER:
        updateRange(xAxisLower, xAxisUpper, yAxisLower, yAxisUpper);
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_LOWER:
        updateRange(xAxisLower, xAxisUpper, yAxisLower, yAxisUpper);
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_UPPER:
        updateRange(xAxisLower, xAxisUpper, yAxisLower, yAxisUpper);
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_LOWER:
        updateRange(xAxisLower, xAxisUpper, yAxisLower, yAxisUpper);
        break;
    case AXIS_EPROPERTIES_NAME::X_GRID_PEN:
        myCustomPlot->xAxis->grid()->setPen(QPen(xGridPen, 2, gridPenStyle));
        break;
    case AXIS_EPROPERTIES_NAME::Y_GRID_PEN:
        myCustomPlot->yAxis->grid()->setPen(QPen(yGridPen, 2, gridPenStyle));
        break;
    case AXIS_EPROPERTIES_NAME::X_SUBGRID_PEN:
        myCustomPlot->xAxis->grid()->setSubGridPen(QPen(xSubGridPen, 1, subGridPenStyle)); // 子网格线(对应子刻度)画笔
        break;
    case AXIS_EPROPERTIES_NAME::Y_SUBGRID_PEN:
        myCustomPlot->yAxis->grid()->setSubGridPen(QPen(ySubGridPen, 1, subGridPenStyle)); // 子网格线(对应子刻度)画笔
        break;
    case AXIS_EPROPERTIES_NAME::ZERO_LINE_PEN:
        myCustomPlot->xAxis->grid()->setZeroLinePen(QPen(zeroLinePen));
        myCustomPlot->yAxis->grid()->setZeroLinePen(QPen(zeroLinePen));
        break;
    case AXIS_EPROPERTIES_NAME::X_BASE_PEN:
        myCustomPlot->xAxis->setBasePen(QPen(xBasePen, basePenWidth));
        break;
    case AXIS_EPROPERTIES_NAME::Y_BASE_PEN:
        myCustomPlot->yAxis->setBasePen(QPen(yBasePen, basePenWidth));
        break;
    case AXIS_EPROPERTIES_NAME::BASE_PEN_WIDTH:
        myCustomPlot->xAxis->setBasePen(QPen(xBasePen, basePenWidth));
        myCustomPlot->yAxis->setBasePen(QPen(yBasePen, basePenWidth));
        break;
    case AXIS_EPROPERTIES_NAME::TICK_PEN:

        myCustomPlot->xAxis->setTickPen(QPen(tickPen, 2));
        myCustomPlot->xAxis->setSubTickPen(QPen(tickPen, 1));
        myCustomPlot->yAxis->setTickPen(QPen(tickPen, 2));
        myCustomPlot->yAxis->setSubTickPen(QPen(tickPen, 1));
        break;
    case AXIS_EPROPERTIES_NAME::LABEL_PEN:
        myCustomPlot->xAxis->setTickLabelColor(labelPen);
        myCustomPlot->xAxis->setLabelColor(labelPen);
        myCustomPlot->yAxis->setTickLabelColor(labelPen);
        myCustomPlot->yAxis->setLabelColor(labelPen);
        break;
    case AXIS_EPROPERTIES_NAME::LABEL_SIZE:
        myCustomPlot->xAxis->setLabelFont(QFont(font().family(), labelSize));
        myCustomPlot->yAxis->setLabelFont(QFont(font().family(), labelSize));
        break;
    case AXIS_EPROPERTIES_NAME::X_SUBGRID_VISIBLE:
        myCustomPlot->xAxis->grid()->setSubGridVisible(xSubGridVisible);
        break;
    case AXIS_EPROPERTIES_NAME::Y_SUBGRID_VISIBLE:
        myCustomPlot->yAxis->grid()->setSubGridVisible(ySubGridVisible);
        break;
    }
    myCustomPlot->replot(QCustomPlot::rpQueuedReplot);

    //若属性有变则更新结构体
    update_property_struct(this->my_plot_property);
}

void DynamicPlot::updateLineProperties(const LINE_EPROPERTIES_NAME propertyName)
{

    switch (propertyName)
    {
    case LINE_EPROPERTIES_NAME::LINE_VISIBLE:
        myGraph[curLineIndex]->setVisible(lineVisible[curLineIndex]);
        break;
    case LINE_EPROPERTIES_NAME::LINES_NAME:
        myGraph[curLineIndex]->setName(linesName[curLineIndex]);
        break;
    case LINE_EPROPERTIES_NAME::LINE_WIDTH:
        linesPen[curLineIndex].setWidth(linesWidth[curLineIndex]);     //曲线粗细
        myGraph[curLineIndex]->setPen(linesPen[curLineIndex]);
        break;
    case LINE_EPROPERTIES_NAME::LINES_COLOR:
        linesPen[curLineIndex].setColor(linesColor[curLineIndex]);     //曲线颜色
        myGraph[curLineIndex]->setPen(linesPen[curLineIndex]);
        break;
    }
    myCustomPlot->replot(QCustomPlot::rpQueuedReplot);
    //若属性有变则更新结构体
    update_property_struct(this->my_plot_property);
}

void DynamicPlot::updateEnumProperties(const ENUM_EPROPERTIES_NAME propertyName)
{
    switch (propertyName) {
    case ENUM_EPROPERTIES_NAME::LINE_PEN_STYLE:
        linesPen[curLineIndex].setStyle(linePenStyle[curLineIndex]);
        myGraph[curLineIndex]->setPen(linesPen[curLineIndex]);
        break;
    case ENUM_EPROPERTIES_NAME::LINE_STYLE:
        myGraph[curLineIndex]->setLineStyle(lineStyle[curLineIndex]);
        break;
    case ENUM_EPROPERTIES_NAME::LINE_SCATTER_STYLE:
        myGraph[curLineIndex]->setScatterStyle(QCPScatterStyle(lineScatterStyle[curLineIndex], scatterSize));
        break;
    case ENUM_EPROPERTIES_NAME::GRID_PEN_STYLE:
        myCustomPlot->xAxis->grid()->setPen(QPen(xGridPen, 2, gridPenStyle));
        myCustomPlot->yAxis->grid()->setPen(QPen(yGridPen, 2, gridPenStyle));
        break;
    case ENUM_EPROPERTIES_NAME::SUBGRID_PEN_STYLE:
        myCustomPlot->xAxis->grid()->setSubGridPen(QPen(xSubGridPen, 1, subGridPenStyle));
        myCustomPlot->yAxis->grid()->setSubGridPen(QPen(ySubGridPen, 1, subGridPenStyle));
        break;
    }
    myCustomPlot->replot(QCustomPlot::rpQueuedReplot);
    //若属性有变则更新结构体
    update_property_struct(this->my_plot_property);
}

void DynamicPlot::updateRange(const double xLower, const double xUpper, const double yLower, const double yUpper)
{

    //cutArrs(data, dataForMagnify, xLower, xUpper, yLower, yUpper);
    calculatesPropertyValue(dataForMagnify, curvePropertiesFM);
    setCurveProperties(curvePropertiesFM);
    myCustomPlot->xAxis->setRange(xLower, xUpper);
    myCustomPlot->yAxis->setRange(yLower, yUpper);

}

void DynamicPlot::magnify(QVector<double> vec)
{
    myCustomPlot->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
    if (magnifyForNewWindow)
    {
        cutArrs(data, dataTemp, vec[0], vec[1], vec[2], vec[3]);
        curvePropertiesTemp.resize(lsName.length());
        calculatesPropertyValue(dataTemp, curvePropertiesTemp);
        emit sendMagnifyInf(myCustomPlot, styleFileVariables, axisVarProperties, vec, data, lsName, curvePropertiesTemp);
    }
    else
    {
        xAxisLower = vec[0];
        xAxisUpper = vec[1];
        yAxisLower = vec[2];
        yAxisUpper = vec[3];
        //        qDebug() << vec[0] << endl << vec[1] << endl << vec[2] << endl << vec[3] << endl;
        updateRange(vec[0], vec[1], vec[2], vec[3]);
    }
    isMagnify = false;
    dataTemp.clear();
}

void DynamicPlot::recover()
{
    myCustomPlot->rescaleAxes(false);
    myCustomPlot->replot(QCustomPlot::rpQueuedReplot);
    calculatesPropertyValue(data, curveProperties);
    setCurveProperties(curveProperties);
}

void DynamicPlot::cutArrs(const QVector<QVector<QCPGraphData> > &vec, QVector<QVector<QCPGraphData> > &result, const double xLower, const double xUpper, const double yLower, const double yUpper)
{
    result.clear();
    result.resize(linesNum);
    lsName.clear();
    lsName.resize(linesNum);
    double e_6 = 1 / (pow(10, 6));
    for (int i = 0; i < linesNum; ++i)
    {
        int left = binaryLookup(vec[i], xLower, false);
        int right = binaryLookup(vec[i], xUpper, false);
        for (int j = left; j <= right; ++j)
        {
            if (vec[i][j].value - yLower > -e_6 && vec[i][j].value - yUpper < e_6)
            {
                result[i].append(vec[i][j]);
            }
        }
        if (!result[i].empty() && magnifyForNewWindow)
        {
            lsName[i] = linesName[i];
        }
    }
}

int DynamicPlot::binaryLookup(const QVector<QCPGraphData> &vec, const double target, const bool isClosest)
{

    int size = vec.size();
    if (target <= vec[0].key)
    {
        return 0;
    }
    if (target >= vec[size - 1].key)
    {
        return size - 1;
    }
    int left, mid, right;
    left = 0;
    right = size - 1;
    const double eps = 1e-6;
    while (left < right)
    {
        mid = left + (right - left) / 2;
        if (abs(vec[mid].key - target) <= eps)
        {
            return mid;
        }
        else if (vec[mid].key < target - eps)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    if (isClosest)
    {
        if (left == 0 || abs(target - vec[left].key) < abs(target - vec[left - 1].key))
        {
            return left;
        }
        else
        {
            return left - 1;
        }
    }
    return left;
}

void DynamicPlot::setStyleFileVariables()
{
    T var;
    var.vPtr = QVariant::fromValue(static_cast<void*>(&fileName) );
    var.name = "窗口标题";
    var.style = VARIABLE_STYLE::STYLE_STRING;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&background0));
    var.name = "背景颜色(上)";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&background1));
    var.name = "背景颜色(下)";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xBasePen));
    var.name = "x坐标轴颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&yBasePen));
    var.name = "y坐标轴颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&basePenWidth));
    var.name = "坐标轴宽度";
    var.style = VARIABLE_STYLE::STYLE_INT;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&tickPen));
    var.name = "刻度线颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&labelPen));
    var.name = "字体颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&labelSize));
    var.name = "字体大小";
    var.style = VARIABLE_STYLE::STYLE_INT;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xGridPen));
    var.name = "x轴网格线颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&yGridPen));
    var.name = "y轴网格线颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xSubGridVisible));
    var.name = "子网格线x可见";
    var.style = VARIABLE_STYLE::STYLE_BOOL;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&ySubGridVisible));
    var.name = "子网格线y可见";
    var.style = VARIABLE_STYLE::STYLE_BOOL;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xSubGridPen));
    var.name = "x轴子网格线颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&ySubGridPen));
    var.name = "y轴子网格线颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&gridPenStyle));
    var.name = "网格线样式";
    var.style = VARIABLE_STYLE::STYLE_PEN_STYLE;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&subGridPenStyle));
    var.name = "子网格线样式";
    var.style = VARIABLE_STYLE::STYLE_PEN_STYLE;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&zeroLinePen));
    var.name = "网格线0颜色";
    var.style = VARIABLE_STYLE::STYLE_COLOR;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xAxisName));
    var.name = "x轴名称";
    var.style = VARIABLE_STYLE::STYLE_STRING;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xAxisUnit));
    var.name = "单位x";
    var.style = VARIABLE_STYLE::STYLE_STRING;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&yAxisName));
    var.name = "y轴名称";
    var.style = VARIABLE_STYLE::STYLE_STRING;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&yAxisUnit));
    var.name = "单位y";
    var.style = VARIABLE_STYLE::STYLE_STRING;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xAxisUpper));
    var.name = "x轴上限";
    var.style = VARIABLE_STYLE::STYLE_DOUBLE;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&xAxisLower));
    var.name = "x轴下限";
    var.style = VARIABLE_STYLE::STYLE_DOUBLE;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&yAxisUpper));
    var.name = "y轴上限";
    var.style = VARIABLE_STYLE::STYLE_DOUBLE;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&yAxisLower));
    var.name = "y轴下限";
    var.style = VARIABLE_STYLE::STYLE_DOUBLE;
    styleFileVariables.insert(var.name, var);

    var.vPtr = QVariant::fromValue(static_cast<void*>(&linesNum));
    var.name = "曲线条数";
    var.style = VARIABLE_STYLE::STYLE_INT;
    styleFileVariables.insert(var.name, var);

    for (int i = 0; i < linesNum; ++i)
    {
        var.vPtr = QVariant::fromValue(static_cast<void*>(&linesName[i]));
        var.name = "曲线名称";
        var.style = VARIABLE_STYLE::STYLE_STRING;
        styleFileVarForLines[i].insert(var.name, var);

        var.vPtr = QVariant::fromValue(static_cast<void*>(&lineVisible[i]));
        var.name = "曲线可见";
        var.style = VARIABLE_STYLE::STYLE_BOOL;
        styleFileVarForLines[i].insert(var.name, var);

        var.vPtr = QVariant::fromValue(static_cast<void*>(&linesColor[i]));
        var.name = "曲线颜色";
        var.style = VARIABLE_STYLE::STYLE_COLOR;
        styleFileVarForLines[i].insert(var.name, var);

        var.vPtr = QVariant::fromValue(static_cast<void*>(&linesWidth[i]));
        var.name = "曲线宽度";
        var.style = VARIABLE_STYLE::STYLE_INT;
        styleFileVarForLines[i].insert(var.name, var);

        var.vPtr = QVariant::fromValue(static_cast<void*>(&linePenStyle[i]));
        var.name = "曲线样式";
        var.style = VARIABLE_STYLE::STYLE_PEN_STYLE;
        styleFileVarForLines[i].insert(var.name, var);

        var.vPtr = QVariant::fromValue(static_cast<void*>(&lineStyle[i]));
        var.name = "曲线连接方式";
        var.style = VARIABLE_STYLE::STYLE_LINE_STYLE;
        styleFileVarForLines[i].insert(var.name, var);

        var.vPtr = QVariant::fromValue(static_cast<void*>(&lineScatterStyle[i]));
        var.name = "曲线节点样式";
        var.style = VARIABLE_STYLE::STYLE_SCATTER_STYLE;
        styleFileVarForLines[i].insert(var.name, var);
    }

}

void DynamicPlot::showTracer(QMouseEvent *event)
{
    if (!isMagnify)
    {
        double x = myCustomPlot->xAxis->pixelToCoord(event->pos().x());

        double y = 0;
        QSharedPointer<QCPGraphDataContainer> tmpContainer;
        for (int i = 0; i < linesNum; ++i)
        {
            tmpContainer = myCustomPlot->graph(i)->data();
            //使用二分法快速查找所在点数据
            int low = 0, high = tmpContainer->size();
            y = 0;
            while(high > low)
            {
                int middle = (low + high) / 2;
                if(x < tmpContainer->constBegin()->mainKey() ||
                        x > (tmpContainer->constEnd()-1)->mainKey())
                    break;

                if(x == (tmpContainer->constBegin() + middle)->mainKey())
                {
                    y = (tmpContainer->constBegin() + middle)->mainValue();
                    break;
                }
                if(x > (tmpContainer->constBegin() + middle)->mainKey())
                {
                    low = middle;
                }
                else if(x < (tmpContainer->constBegin() + middle)->mainKey())
                {
                    high = middle;
                }
                if(high - low <= 1)
                {   //差值计算所在位置数据
                    y = (tmpContainer->constBegin()+low)->mainValue() +
                            ( (x - (tmpContainer->constBegin() + low)->mainKey()) *
                              ((tmpContainer->constBegin()+high)->mainValue() - (tmpContainer->constBegin()+low)->mainValue()) ) /
                            ( (tmpContainer->constBegin()+high)->mainKey() - (tmpContainer->constBegin()+low)->mainKey());
                    break;
                }

            }
            //显示y轴的鼠标动态坐标，缺点无法定位xy所以无法附加单位，附加单位仍需继续修改setText传参
            if (!m_TracersY[i].isNull())
            {
                m_TracersY[i]->updatePosition(x, y);
                m_TracersY[i]->setText(QString::number(y, 'f', 2));
            }
        }
        //显示x轴的鼠标动态坐标
        m_TracerX->updatePosition(x, 0);
        m_TracerX->setText(QString::number(x, 'f', 2));
        myCustomPlot->replot(QCustomPlot::rpQueuedReplot);
    }

}

void DynamicPlot::closeEvent(QCloseEvent *event)
{
    event->accept();
}
//读配置文件
void DynamicPlot::on_action_open_triggered()
{
    styleFileName = QFileDialog::getOpenFileName(this, tr("选择样式文件"), "", tr("TEXT(*.txt)"));
    if (!styleFileName.isEmpty())
        readPropertiesFile(styleFileName);
}
//保存配置文件
void DynamicPlot::on_action_save_triggered()
{
    //    if (styleFileName.isEmpty())
    //    {
    //        styleFileName = QFileDialog::getSaveFileName(this, tr("保存样式文件"), styleFileName, tr("TEXT(*.txt)"));
    //    }
    writePropertiesFile(styleFileName);
}

void DynamicPlot::on_action_save_as_triggered()
{
    styleFileName = QFileDialog::getSaveFileName(this, tr("保存样式文件"), "", tr("TEXT(*.txt)"));
    if (!styleFileName.isEmpty())
        writePropertiesFile(styleFileName);
}

void DynamicPlot::on_action_hide_triggered()
{
    if(!side_widget_flag && buttonRelesed) //侧边栏显示状态，隐藏
    {
        buttonRelesed = false;
        tableAnimation->setStartValue(QRect(this->rect().width() - ui->tableWidget->width() - sideToolBar->width() - 5, 0, ui->tableWidget->width(), ui->tableWidget->height()));
        tableAnimation->setEndValue(QRect(this->rect().width() + sideToolBar->width(), 0, ui->tableWidget->width(), ui->tableWidget->height()));
        tableAnimation->start();
        plotAnimation->setStartValue(QRect(this->rect().width() - ui->plotWidget->width() - ui->tableWidget->width() - sideToolBar->width() - 10, 0, ui->plotWidget->width(), ui->plotWidget->height()));
        plotAnimation->setEndValue(QRect(this->rect().width() - ui->plotWidget->width() - ui->tableWidget->width() - sideToolBar->width() - 10, 0, ui->plotWidget->width() + ui->tableWidget->width() + 5, ui->plotWidget->height()));
        plotAnimation->start();
    }
}

void DynamicPlot::on_action_unfold_triggered()
{
    if(side_widget_flag && buttonRelesed) //侧边栏隐藏状态，显示
    {
        buttonRelesed = false;
        plotAnimation->setStartValue(QRect(this->rect().width() - ui->plotWidget->width() - sideToolBar->width() - 5, 0, ui->plotWidget->width(), ui->plotWidget->height()));
        plotAnimation->setEndValue(QRect(this->rect().width() - ui->plotWidget->width() - sideToolBar->width() - 5, 0, ui->plotWidget->width() - ui->tableWidget->width() - 5, ui->plotWidget->height()));
        plotAnimation->start();
        tableAnimation->setStartValue(QRect(this->rect().width(), 0, ui->tableWidget->width(), ui->tableWidget->height()));
        tableAnimation->setEndValue(QRect(this->rect().width() - ui->tableWidget->width() - sideToolBar->width() - 5, 0 , ui->tableWidget->width(), ui->tableWidget->height()));
        tableAnimation->start();
    }
}

void DynamicPlot::animationFinished()
{
    side_widget_flag = !side_widget_flag;
    buttonRelesed = true;
}

void DynamicPlot::on_action_magnify_triggered()
{
    isMagnify = true;
    magnifyForNewWindow = false;
    myCustomPlot->setInteraction(QCP::iRangeDrag,false);//取消拖动
    myCustomPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
}

void DynamicPlot::on_action_new_win_magnify_triggered()
{
    isMagnify = true;
    magnifyForNewWindow = true;
    //    myCustomPlot->setInteractions(QCP::iSelectPlottables| QCP::iMultiSelect);
    myCustomPlot->setSelectionRectMode(QCP::SelectionRectMode::srmCustom);
}

void DynamicPlot::on_action_recover_triggered()
{
    recover();
}

