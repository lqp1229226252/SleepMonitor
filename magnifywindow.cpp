#include "magnifywindow.h"
#include "ui_magnifywindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

magnifyWindow::magnifyWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::magnifyWindow)
{
    ui->setupUi(this);
}

magnifyWindow::magnifyWindow(
        QCustomPlot* customPlot,
        QMap<QString, T>& styleFileVariables,
        QMap<QtVariantProperty *, AxisPropertyIndex> axisVarProperties,
        QVector<double> &vec,
        QVector<QVector<QCPGraphData>>& data,
        const QVector<QString> names,
        const QVector<QVector<double>>& properties,
        QWidget *parent) :
    QWidget(parent),
    ui(new Ui::magnifyWindow)
{
    ui->setupUi(this);

    otherCustomPlot = customPlot;
    this->customPlot = ui->plotWidget;
    linesName = names;
    curveProperties = properties;

    this->customPlot->xAxis->setRange(vec[0], vec[1]);
    this->customPlot->yAxis->setRange(vec[2], vec[3]);
    this->customPlot->xAxis->setTickLengthIn(3);                     //轴线内刻度的长度
    this->customPlot->xAxis->setTickLengthOut(5);                    //轴线外刻度的长度
    this->customPlot->yAxis->setTickLengthIn(3);                     //轴线内刻度的长度
    this->customPlot->yAxis->setTickLengthOut(5);                    //轴线外刻度的长度
    this->customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);//可读性优于设置
    this->customPlot->xAxis->setNumberFormat("gbc");//g灵活的格式,b漂亮的指数形式，c乘号改成×
    this->customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);//可读性优于设置
    this->customPlot->yAxis->setNumberFormat("gbc");//g灵活的格式,b漂亮的指数形式，c乘号改成×

    //设置画布背景颜色
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    QMapIterator<QtVariantProperty *, AxisPropertyIndex> axisIterator(axisVarProperties);
    while (axisIterator.hasNext())
    {
        axisIterator.next();
        if (!QString::compare(axisIterator.value().variableName, "背景颜色(上)"))
        {
            plotGradient.setColorAt(0, *(static_cast<QColor*>(styleFileVariables["背景颜色(上)"].vPtr.value<void*>())));
        }
        else if (!QString::compare(axisIterator.value().variableName, "背景颜色(下)"))
        {
            plotGradient.setColorAt(1, *(static_cast<QColor*>(styleFileVariables["背景颜色(下)"].vPtr.value<void*>())));
        }
        else
        {
            updateAxisProperties(axisIterator.value().first);
        }
    }
    this->customPlot->setBackground(plotGradient);
    for (int i = 0, k = 0; i < linesName.length(); ++i)
    {
        if (0 != QString::compare(linesName[i], ""))
        {
            addLineGraph(this->customPlot, otherCustomPlot->graph(i), data[i], k++);
        }
    }
    setProperties();
    this->customPlot->replot(QCustomPlot::rpQueuedReplot);
    setToolBar();

    //------------------------------
    side_widget_flag = false;
    buttonRelesed = true;
    tableAnimation = new QPropertyAnimation(ui->tableWidget,"geometry");
    tableAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    tableAnimation->setDuration(1000);
    plotAnimation = new QPropertyAnimation(ui->plotWidget,"geometry");
    plotAnimation->setEasingCurve(QEasingCurve::InOutQuint);
    plotAnimation->setDuration(1000);
    connect(plotAnimation, &QPropertyAnimation::finished, this, &magnifyWindow::animationFinished);
}

magnifyWindow::~magnifyWindow()
{
    delete ui;

    delete customPlot;

    if (tableAnimation != nullptr)
    {
        delete tableAnimation;
    }
    if (plotAnimation != nullptr)
    {
        delete plotAnimation;
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
}

void magnifyWindow::setToolBar()
{
    sideToolBar = new QWidget();
    sideToolBar->setObjectName("sideToolBar");
    sideToolBar->setStyleSheet(QString::fromUtf8("#sideToolBar{border:1px solid #a2a2a2}"));
    sideToolBar->setFixedWidth(30);
//    menuDialog->setWindowFlags(Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    toolBar = new QToolBar("");
    toolBar->addAction(ui->action_hide);
    toolBar->addAction(ui->action_unfold);
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

void magnifyWindow::updateAxisProperties(const AXIS_EPROPERTIES_NAME propertyName)
{
    switch (propertyName) {
    case AXIS_EPROPERTIES_NAME::FILE_NAME:
    case AXIS_EPROPERTIES_NAME::BACKGROUND_0:
    case AXIS_EPROPERTIES_NAME::BACKGROUND_1:
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_NAME:
    case AXIS_EPROPERTIES_NAME::X_AXIS_UNIT:
        customPlot->xAxis->setLabel(otherCustomPlot->xAxis->label());
        break;
    case AXIS_EPROPERTIES_NAME::Y_AXIS_NAME:
    case AXIS_EPROPERTIES_NAME::Y_AXIS_UNIT:
        customPlot->yAxis->setLabel(otherCustomPlot->yAxis->label());
        break;
    case AXIS_EPROPERTIES_NAME::X_AXIS_UPPER:
    case AXIS_EPROPERTIES_NAME::X_AXIS_LOWER:
    case AXIS_EPROPERTIES_NAME::Y_AXIS_UPPER:
    case AXIS_EPROPERTIES_NAME::Y_AXIS_LOWER:
        break;
    case AXIS_EPROPERTIES_NAME::X_GRID_PEN:
        customPlot->xAxis->grid()->setPen(otherCustomPlot->xAxis->grid()->pen());
        break;
    case AXIS_EPROPERTIES_NAME::Y_GRID_PEN:
        customPlot->yAxis->grid()->setPen(otherCustomPlot->yAxis->grid()->pen());
        break;
    case AXIS_EPROPERTIES_NAME::X_SUBGRID_PEN:
        customPlot->xAxis->grid()->setSubGridPen(otherCustomPlot->xAxis->grid()->subGridPen());
        break;
    case AXIS_EPROPERTIES_NAME::Y_SUBGRID_PEN:
        customPlot->yAxis->grid()->setSubGridPen(otherCustomPlot->yAxis->grid()->subGridPen());
        break;
    case AXIS_EPROPERTIES_NAME::ZERO_LINE_PEN:
        customPlot->xAxis->grid()->setZeroLinePen(otherCustomPlot->xAxis->grid()->zeroLinePen());
        customPlot->yAxis->grid()->setZeroLinePen(otherCustomPlot->yAxis->grid()->zeroLinePen());
        break;
    case AXIS_EPROPERTIES_NAME::X_BASE_PEN:
        customPlot->xAxis->setBasePen(otherCustomPlot->xAxis->basePen());
        break;
    case AXIS_EPROPERTIES_NAME::Y_BASE_PEN:
    case AXIS_EPROPERTIES_NAME::BASE_PEN_WIDTH:
        customPlot->yAxis->setBasePen(otherCustomPlot->yAxis->basePen());
        break;
    case AXIS_EPROPERTIES_NAME::TICK_PEN:
        customPlot->xAxis->setTickPen(otherCustomPlot->xAxis->tickPen());
        customPlot->xAxis->setSubTickPen(otherCustomPlot->xAxis->subTickPen());
        customPlot->yAxis->setTickPen(otherCustomPlot->yAxis->tickPen());
        customPlot->yAxis->setSubTickPen(otherCustomPlot->yAxis->subTickPen());
        break;
    case AXIS_EPROPERTIES_NAME::LABEL_PEN:
        customPlot->xAxis->setTickLabelColor(otherCustomPlot->xAxis->tickLabelColor());
        customPlot->xAxis->setLabelColor(otherCustomPlot->xAxis->labelColor());
        customPlot->yAxis->setTickLabelColor(otherCustomPlot->yAxis->tickLabelColor());
        customPlot->yAxis->setLabelColor(otherCustomPlot->yAxis->labelColor());
        break;
    case AXIS_EPROPERTIES_NAME::LABEL_SIZE:
        customPlot->xAxis->setLabelFont(otherCustomPlot->xAxis->labelFont());
        customPlot->yAxis->setLabelFont(otherCustomPlot->yAxis->labelFont());
        break;
    case AXIS_EPROPERTIES_NAME::X_SUBGRID_VISIBLE:
        customPlot->xAxis->grid()->setSubGridVisible(otherCustomPlot->xAxis->grid()->subGridVisible());
        break;
    case AXIS_EPROPERTIES_NAME::Y_SUBGRID_VISIBLE:
        customPlot->yAxis->grid()->setSubGridVisible(otherCustomPlot->yAxis->grid()->subGridVisible());
        break;
    }
}

void magnifyWindow::addLineGraph(QCustomPlot *customPlot, QCPGraph* graph, const QVector<QCPGraphData> &vector, const int index)
{
    QCPGraph* myGraph = customPlot->addGraph();
    QSharedPointer<QCPGraphDataContainer> dataContainer;
    dataContainer = myGraph->data();
    dataContainer->set(vector, true);
    if (index == 0)
    {
        m_TracerX = QPointer<MyTracer> (new MyTracer(customPlot, customPlot->graph(0), TracerType::XAxisTracer));
        connect(this->customPlot, &QCustomPlot::mousePress, this, &magnifyWindow::showTracer);
    }
    m_TracersY.append(QPointer<MyTracer> (new MyTracer(customPlot, customPlot->graph(index), TracerType::DataTracer)));
    myGraph->setVisible(graph->visible());
    myGraph->setPen(graph->pen());
    myGraph->setLineStyle(graph->lineStyle());
    myGraph->setScatterStyle(graph->scatterStyle());
    myGraph->setAntialiasedFill(true);
    myGraph->setName(graph->name());
}

void magnifyWindow::setProperties()
{
    QHBoxLayout *layout = new QHBoxLayout(ui->tableWidget);
    layout->setMargin(0);
    propertyBrowser = new QtTreePropertyBrowser(ui->tableWidget);
    layout->addWidget(propertyBrowser);
    propertyBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);    //设置表头内容自动调整
    for (int i = 0; i < linesName.size(); ++i)
    {
        if (0 != QString::compare(linesName[i], ""))
            setLineProperties(i);
    }
}

void magnifyWindow::setLineProperties(const int index)
{
    QtVariantPropertyManager* pVarManager = new QtVariantPropertyManager(propertyBrowser);
    QtProperty* propertyBar = pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("曲线属性"));
    QtVariantProperty* item = pVarManager->addProperty(QVariant::String, QStringLiteral("曲线名称"));
    item->setValue(linesName[index]);
    propertyBar->addSubProperty(item);

    //不可更改的属性，最大最小值、PV/RMS、平均值；
    item = pVarManager->addProperty(QVariant::Double, QStringLiteral("最大值"));
    item->setValue(curveProperties[index][0]);
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Double, QStringLiteral("最小值"));
    item->setValue(curveProperties[index][1]);
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Double, QStringLiteral("平均值"));
    item->setValue(curveProperties[index][2]);
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Double, QStringLiteral("PV值"));
    item->setValue(curveProperties[index][3]);
    propertyBar->addSubProperty(item);

    item = pVarManager->addProperty(QVariant::Double, QStringLiteral("RMS值"));
    item->setValue(curveProperties[index][4]);
    propertyBar->addSubProperty(item);

    propertyBrowser->addProperty(propertyBar);
}

void magnifyWindow::showTracer(QMouseEvent *event)
{
    double x = customPlot->xAxis->pixelToCoord(event->pos().x());

    double y = 0;
    QSharedPointer<QCPGraphDataContainer> tmpContainer;
    for (int i = 0; i < customPlot->graphCount(); ++i)
    {
        tmpContainer = customPlot->graph(i)->data();
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
        m_TracersY[i]->updatePosition(x, y);
        m_TracersY[i]->setText(QString::number(y, 'f', 2));

    }
    //显示x轴的鼠标动态坐标
    m_TracerX->updatePosition(x, 0);
    m_TracerX->setText(QString::number(x, 'f', 2));
    customPlot->replot(QCustomPlot::rpQueuedReplot);
}

void magnifyWindow::on_action_hide_triggered()
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

void magnifyWindow::on_action_unfold_triggered()
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

void magnifyWindow::animationFinished()
{
    side_widget_flag = !side_widget_flag;
    buttonRelesed = true;
}
