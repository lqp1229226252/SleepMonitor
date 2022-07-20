#include "sleepmonitor.h"
#include "ui_sleepmonitor.h"

SleepMonitor::SleepMonitor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SleepMonitor)
{
    ui->setupUi(this);
}

SleepMonitor::~SleepMonitor()
{
    delete ui;
}

