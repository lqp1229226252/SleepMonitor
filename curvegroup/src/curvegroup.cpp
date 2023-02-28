#include "curvegroup.h"
#define defalut_curve_num 5
#define defalut_max_point 200
CurveGroup::CurveGroup(QWidget *parent):QWidget(parent)
{
    init();
    setToolStyle();
    setConnect();
    initLayout();
}

CurveGroup::~CurveGroup()
{
    delete  curvewnd;
    delete  widget_layout;
    qDeleteAll(seriespoint);
    seriespoint.clear();
    delete last_group;
    delete next_group;
}

void CurveGroup::init()
{
    curvewnd=new CurveWnd(this);
    this->curvewnd->lower();

    this->widget_layout=new QHBoxLayout();
    this->last_group=new QToolButton(this);
    this->next_group=new QToolButton(this);

    this->curve_num_group=defalut_curve_num;
    this->curve_num=defalut_curve_num;
    this->group_num=1;
    this->min=0;
    this->max=0;
    this->max_point=defalut_max_point;
    this->current_group=0;
}

void CurveGroup::setCurveNumGroup(int num)
{
    this->curve_num_group=num;
    this->curvewnd->setCurveNum(num);
}

void CurveGroup::setCurveNum(int num)
{
    this->curve_num=num;
    this->group_num=(curve_num-1)/curve_num_group;
    //初始化曲线数据
    for(int i=0;i<num;i++)
    {
        SeriesPoint *seriepoint=new SeriesPoint();
        seriepoint->setMaxPointNum(this->max_point);
        seriespoint.append(seriepoint);
    }
}

void CurveGroup::setMinMax(int min, int max)
{
    this->min=min;
    this->max=max;
    this->curvewnd->setMinMax(min,max);
}

void CurveGroup::setMaxPoint(int num)
{
    this->max_point=num;
    this->curvewnd->setMaxPoint(num);
}

void CurveGroup::setCurveLabels(QStringList labels)
{
    this->labels=labels;
    QStringList show_labels;
    for(int i=0;i<curve_num_group;i++)
    {
        show_labels.append(labels[i+current_group*curve_num_group]);
    }
    this->curvewnd->setCurveLabels(show_labels);
}

void CurveGroup::append(QList<float> data)
{
    if(data.size()!=curve_num)
    {
//        throw QString('the size of data != num of curve');
    }
    QList<float> show_data;
    QList<float>::const_iterator begin=data.begin();
    QList<float>::const_iterator::difference_type n=current_group*curve_num_group;
    for(int i=0;i<curve_num_group;i++)
    {
        show_data.append(*(begin+n));
        begin++;
    }
    curvewnd->append(show_data);
//    qDebug()<<show_data;
//    qDebug()<<data;
//    for(int i=0;i<data.size();i++)
//    {
//        seriespoint[i]->append(data[i]);
//    }
}

void CurveGroup::next()
{
    if(current_group==group_num)
    {
        return;
    }
    else
    {
        this->curvewnd->clear();
        this->current_group++;
        QStringList show_labels;
        for(int i=0;i<curve_num_group;i++)
        {
            show_labels.append(labels[i+current_group*curve_num_group]);
        }
        this->curvewnd->updateCurveLabels(show_labels);
        if(current_group==group_num)
        {
            emit end_group();
        }
    }


}

void CurveGroup::last()
{
    if(current_group==0)
    {
        return;
    }
    else
    {
        this->curvewnd->clear();
        this->current_group--;
        QStringList show_labels;
        for(int i=0;i<curve_num_group;i++)
        {
            show_labels.append(labels[i+current_group*curve_num_group]);
        }
        this->curvewnd->updateCurveLabels(show_labels);
        if(current_group==0)
        {
            emit first_group();
        }
    }
}

void CurveGroup::timerEvent(QTimerEvent *event)
{
    //模拟数据
    QList<float> data;
    int y=rand();
    for (int i = 0; i < this->curve_num; i++)
    {
        float chart_data = cos(3.14 * y)+2*cos(3.14*y*2);
        data.append(chart_data);
    }
    this->append(data);
}

void CurveGroup::resizeEvent(QResizeEvent *event)
{
    this->last_group->move(0,this->height()/2-last_group->height()/2);
    this->next_group->move(this->width()-last_group->width(),this->height()/2-last_group->height()/2);
}

void CurveGroup::test_chart()
{
    startTimer(1);
}

void CurveGroup::initLayout()
{
    this->widget_layout->addWidget(curvewnd);
    this->setLayout(widget_layout);
}

void CurveGroup::setToolStyle()
{
    this->last_group->resize(50,50);
    this->next_group->resize(50,50);
    this->last_group->raise();
    this->last_group->move(0,this->height()/2-last_group->height()/2);
    this->next_group->raise();
    this->next_group->move(this->width()-last_group->width(),this->height()/2-last_group->height()/2);

    this->last_group->setIconSize(QSize(50,50));
    this->last_group->setIcon(QIcon("./res/last.png"));
    this->next_group->setIconSize(QSize(50,50));
    this->next_group->setIcon(QIcon("./res/next.png"));
    this->last_group->setStyleSheet("QToolButton{background: transparent; }");
    this->next_group->setStyleSheet("QToolButton{background: transparent; }");
}

void CurveGroup::setConnect()
{
    connect(last_group,&QToolButton::clicked,this,&CurveGroup::last);
    connect(next_group,&QToolButton::clicked,this,&CurveGroup::next);

}

