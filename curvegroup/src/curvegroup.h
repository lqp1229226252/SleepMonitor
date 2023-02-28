#ifndef CURVEGROUP_H
#define CURVEGROUP_H
#include "QWidget"
#include "curvewnd.h"
#include "seriespoint.h"
class CurveGroup:public QWidget
{
    Q_OBJECT
public:
    explicit CurveGroup(QWidget *parent=nullptr);
    ~CurveGroup();
    void init();
    //设置每组显示的曲线数目
    void setCurveNumGroup(int);
    //设置显示的曲线数目
    void setCurveNum(int num);
    //设置y的范围
    void setMinMax(int,int);
    //设置显示的数目
    void setMaxPoint(int);
    //设置标签
    void setCurveLabels(QStringList);

    //数据插入
    void append(QList<float>);
    void timerEvent(QTimerEvent *event);
    void resizeEvent(QResizeEvent *event);
    //测试数据
    void test_chart();
public slots:
    void next();
    void last();
signals:
    void first_group();
    void end_group();
private:
    CurveWnd *curvewnd;
    QHBoxLayout *widget_layout;
    int curve_num_group;
    int curve_num;
    int group_num;
    int  min,max;
    int max_point;
    QList<SeriesPoint *> seriespoint;
    QStringList labels;
    QToolButton *next_group;
    QToolButton *last_group;
    int current_group;
    void initLayout();
    void setToolStyle();
    void setConnect();
};

#endif // CURVEGROUP_H
