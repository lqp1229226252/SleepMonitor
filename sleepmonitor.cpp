#include "sleepmonitor.h"
#include "ui_sleepmonitor.h"

SleepMonitor::SleepMonitor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SleepMonitor)
{
    ui->setupUi(this);
    ui->widget_eegwnd->setFixedWidth(1100);
    this->setFixedSize(1920,1000);
    ui->widget_controlWnd->setFixedWidth(800);
    ui->widget_result->setFixedWidth(800);
    ui->widget_outPutWnd->setFixedWidth(800);
    ui->widget_controlWnd->get_eegwnd(ui->widget_eegwnd,ui->widget_outPutWnd);

}



SleepMonitor::~SleepMonitor()
{
    delete ui;
}

