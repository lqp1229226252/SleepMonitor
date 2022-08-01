#ifndef EEGWND_H
#define EEGWND_H

#include <QWidget>
#include <QVector>
#include <QString>
#include "plotwidget.h"

namespace Ui {
class EEGWnd;
}

class EEGWnd : public QWidget
{
    Q_OBJECT

public:
    explicit EEGWnd(QWidget *parent = nullptr);
    ~EEGWnd();
    //接口
    //空
    void SetPlotAmount(int num=16);
    void Initialize(void);
    void AddData(QVector<double> data);
    void init(void);
    void timerEvent(QTimerEvent *ev);
    void send_EEGWnd_size(void);
    //设置16条曲线名称
    void setLineName(QVector<QString> names);
    void send_16PlotWidget_position(void);
    //获取布局中窗口的大小，布局中的窗口如果直接获取的话不管在哪都是(0,0)
    void get_plotwidget_position(void);


private:
    Ui::EEGWnd *ui;
    QVector<PlotWidget*> m_PlotWidgetPtrs;
    QVector<QRect> plotwidget_position;

};

#endif // EEGWND_H
