#include "sleepmonitor.h"
#include "ui_sleepmonitor.h"

SleepMonitor::SleepMonitor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SleepMonitor)
{
    ui->setupUi(this);
    ui->widget_controlWnd->get_eegwnd(ui->curvegroup,ui->widget_outPutWnd);
    ui->curvegroup->setMaxPoint(100);
    ui->curvegroup->setMinMax(-1,1);
    ui->curvegroup->setCurveNumGroup(8);
    ui->curvegroup->setCurveNum(16);
    QStringList labels={
            "FP1脑电","FP2脑电","红光","近红光","绿光",
            "坐立角","翻滚角","运动加速度","左声道鼾声",
            "右声道鼾声","x-gro","y-gro","z-gro","x-acc",
            "y-acc","z-acc"
        };
    ui->curvegroup->setCurveLabels(labels);

}



SleepMonitor::~SleepMonitor()
{
    delete ui;
}

