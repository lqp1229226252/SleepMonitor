#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = nullptr);
    ~PlotWidget();
    void Initialize(void);
    void SetValues (double val);
    void AddDatum(double val);
    void timerEvent(QTimerEvent *event);

    //上层调用这个函数设置曲线名称
    void setLineName(QString name);

    //id 标识自己是第几条曲线
    void from_eegwnd_QVector_PlotWidgets(QVector<PlotWidget*> pltwidgets,int id);
    //缩放窗口
    void zoomPlotWidget(void);
    //最大化窗口
    void Max_Widget(void);
    //恢复窗口
    void Recovery_Widget(void);
    //隐藏曲线
    void hide_plotWidget(bool f);
    //获取EEGWnd窗口大小函数
    void get_EEGWnd_Size(QSize size);
    //获取自己最新的相对位置
    void get_newest_position_from_EEGWnd(QRect rect);

private slots:
    void clicked_maxmum(void);
    void clicked_attribute(void);
private:
    Ui::PlotWidget *ui;
    //是否最大化标志
    bool flag_maxmum = false;
    int id;
    QVector<PlotWidget*> plotwidgets;
    //EEGWnd窗口大小
    QSize EEGWnd_size;
    //PlotWidget窗口大小
    QSize PlotWidget_size;
    //自己相对于EEGWnd最新的位置
    QRect position_in_EEGWnd;
};

#endif // PLOTWIDGET_H
