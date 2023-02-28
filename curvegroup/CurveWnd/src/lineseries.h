#ifndef LINESERIES_H
#define LINESERIES_H

#include <QLineSeries>
#include "QPointF"
#include "maxwidget.h"
#include "QThread"
using namespace QtCharts;
class LineSeries:public QLineSeries
{
    Q_OBJECT
public:
    LineSeries(QObject *parent=nullptr);
    ~LineSeries();
    void init();
    void setConnect();
    void setMaxPointNum(int);
    void appendData(float);
    void setId(int id);
    void setMinMax(int,int);
    void clear();
public slots:
    void maxCurve(QString);
    void deelPointReplace(int index);
    void closeWidget();
signals:
    void updateMaxCurve(QList<QPointF> points,int id);
private:
    int max_point_num;
    int point_index;
    //曲线画笔
    QPen pen;
    //曲线最大化
    Maxwidget *curvemax;
    bool max_falg;
    //曲线Id
    int Id;
    //曲线的最大值和最小值
    int max,min;

    QList<QPointF> raw_points;
};

#endif // LINESERIES_H
