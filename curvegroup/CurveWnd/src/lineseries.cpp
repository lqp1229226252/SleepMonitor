#include "lineseries.h"
#include "QDebug"
LineSeries::LineSeries(QObject *parent):QLineSeries(parent)
{
    init();
    pen.setColor(Qt::black);
    this->setPen(pen);
}

LineSeries::~LineSeries()
{
    delete curvemax;
}

void LineSeries::init()
{
    this->point_index=0;
    this->max_point_num=100;
    curvemax=new Maxwidget();
    max_falg=false;
    setConnect();
}

void LineSeries::setConnect()
{
//    connect(this,&LineSeries::doubleClicked,this,&LineSeries::maxCurve);
    connect(this,&LineSeries::pointReplaced,this,&LineSeries::deelPointReplace);
    connect(this,&LineSeries::pointAdded,this,&LineSeries::deelPointReplace);
    connect(curvemax,&Maxwidget::closed,this,&LineSeries::closeWidget);
}

void LineSeries::setMaxPointNum(int num)
{
    this->max_point_num = num;
}

void LineSeries::appendData(float data)
{
    QList<QPointF> series_point=this->points();
    if (series_point.size()<max_point_num)
    {
        //原始数据
        QPointF point(series_point.size(), data);
        this->raw_points.append(point);
        //图标显示数据
        data=data/(max-min);
        point.setY(data+2*Id);
        this->append(point);
        return;
    }
    if (point_index == max_point_num)
    {
        point_index = 0;
    }
    //原始数据
    QPointF point(point_index, data);
    this->raw_points.replace(point_index,point);
    //图标显示数据
    data=data/(max-min);
    point.setY(data+2*Id);
    this->replace(point_index, point);
    point_index++;
}

void LineSeries::setId(int id)
{
    this->Id=id;
}

void LineSeries::setMinMax(int min, int max)
{
    this->min=min;
    this->max=max;
}

void LineSeries::clear()
{
    this->raw_points.clear();
    QLineSeries::clear();
}

void LineSeries::maxCurve(QString title)
{
    max_falg=true;
    curvemax->setRangeY(min,max);
    curvemax->setRangeX(0,max_point_num);
    curvemax->setText(title);
    curvemax->clear();
    curvemax->setPoints(raw_points);
    curvemax->show();
}

void LineSeries::deelPointReplace(int index)
{
    if(max_falg)
    {
        QList<QPointF> points=this->points();
        if(curvemax->point_size()<=max_point_num)
        {
            this->curvemax->setPoint(raw_points.last());
        }
        else
        {
            this->curvemax->setPoint(index,this->raw_points[index]);
        }

    }
}

void LineSeries::closeWidget()
{
    this->max_falg=false;
    this->curvemax->hide();
}
