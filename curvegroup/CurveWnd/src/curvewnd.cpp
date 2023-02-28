#include "curvewnd.h"
#include "math.h"
#include "QDebug"
#include "lineseries.h"
void CurveWnd::initParam()
{
    //初始化控件
    //初始化图表
    this->chart=new QChart();
    this->chart->legend()->hide();
    this->chart->setMargins(QMargins(0,0,0,0));
    this->chart->setTheme(QChart::ChartThemeLight);
    //初始化图表视图
    this->view=new QChartView(this->chart);
    //初始化坐标
    this->axisx=new QCategoryAxis;
    this->axisx->setLabelsVisible(false);
    this->axisx->setGridLineVisible(false);
    this->chart->addAxis(axisx,Qt::AlignBottom);
    this->axisy=new QCategoryAxis;
    this->axisy->setLabelsVisible(true);
    this->chart->addAxis(axisy,Qt::AlignLeft);
    //初始化画笔
    this->pen.setColor(Qt::black);
    this->pen.setWidth(1);
    //初始化布局
    this->layout=new QHBoxLayout();
    this->layout->setMargin(0);
    this->layout->addWidget(view);
    this->setLayout(this->layout);
    //初始化参数
    this->max_point_num=50;
    this->max=0;
    this->min=0;
}
void CurveWnd::timerEvent(QTimerEvent *event)
{
    //模拟数据
    QList<float> data;
    int y=rand();
    for (int i = 0; i < this->series_num; i++)
    {
        float chart_data = cos(3.14 * y)+2*cos(3.14*y*2);
        data.append(chart_data);
    }
    this->append(data);
    this->updateChart();
}

void CurveWnd::mouseDoubleClickEvent(QMouseEvent *event)
{
    int y=event->y();
    int curve_with=this->height()/this->series_num;
    int index=series_num-y/curve_with-1;
    series[index]->maxCurve(labels[index]);

}
CurveWnd::CurveWnd(QWidget*parent):QWidget(parent)
{
    initParam();
}
CurveWnd::~CurveWnd()
{
    series.clear();
    qDeleteAll(series);
    delete chart;
    delete view;
    delete layout;
    delete axisx;
    delete axisy;
}
void CurveWnd::setAxisXRange(int start,int end)
{
    //设置x坐标的范围
    this->axisx->setRange(start,end);
    //设置最大显示数目
    this->max_point_num=end;
}
void CurveWnd::setAxisYRange(int start,int end)
{
    //设置y坐标的范围
    this->axisy->setRange(start,end);
}
void CurveWnd::setMaxPoint(int num)
{
    this->max_point_num=num;
    this->axisx->setRange(0,num);
}
void CurveWnd::setCurveLabels(QStringList labels)
{
    //判定标签数目和曲线数目是否一致
    if (labels.size() != this->series_num)
    {
        throw QString("labels of size != curve of size");
    }
    //设置坐标标签
    for (int i = 0; i < this->series_num; i++)
    {
        this->axisy->append(labels[i], i * 2);
    }
    //使标签与坐标对齐
    this->axisy->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    this->labels=labels;
}

void CurveWnd::updateCurveLabels(QStringList labels)
{
    //判定标签数目和曲线数目是否一致
    if (labels.size() != this->series_num)
    {
        throw QString("labels of size != curve of size");
    }
    //设置坐标标签
    for (int i = 0; i < this->series_num; i++)
    {
        this->axisy->replaceLabel(this->labels[i],labels[i]);
    }
    this->labels=labels;
//    qDebug()<<this->labels;
}
void CurveWnd::setCurveNum(int num)
{
    if(max==0)
    {
        throw QString("please set min and max");
    }
    //设置y坐标大小
    this->setAxisYRange(-1, (num * 2) - 1);
    //设置曲线数目
    this->series_num=num;
    //初始化曲线数据
    for(int i=0;i<num;i++)
    {
        LineSeries *curve=new LineSeries();
        //设置曲线数据最大数目

        curve->setMaxPointNum(this->max_point_num);
        curve->setId(i);
        curve->setMinMax(min,max);
        this->chart->addSeries(curve);
        curve->attachAxis(axisx);
        curve->attachAxis(axisy);
        series.append(curve);
    }
}
void CurveWnd::append(QList<float> data)
{   
    //判定插入数据和曲线数目是否一致
    if (data.size() != this->series_num)
    {
        throw QString("data of size != curve of size");
    }
    QList<LineSeries *>::iterator begin=series.begin();
    for(int i=0;i<this->series_num;i++)
    {
        float chart_data = data[i];
        (*begin)->appendData(chart_data);
        begin++;
    }
}

void CurveWnd::clear()
{
    QList<LineSeries *>::iterator begin=series.begin();
    for(int i=0;i<this->series_num;i++)
    {
        (*begin)->clear();
        begin++;
    }
}
void CurveWnd::updateChart()
{
     this->chart->update();
}
void CurveWnd::test_chart()
{
    startTimer(1);
}

void CurveWnd::setMinMax(int min, int max)
{
    this->min=min;
    this->max=max;
}
