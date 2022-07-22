#ifndef SLEEPMONITOR_H
#define SLEEPMONITOR_H

#include <QMainWindow>

//#include "eegwnd.h"

#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif

QT_BEGIN_NAMESPACE
namespace Ui { class SleepMonitor; }
QT_END_NAMESPACE

class SleepMonitor : public QMainWindow
{
    Q_OBJECT

public:
    SleepMonitor(QWidget *parent = nullptr);
    ~SleepMonitor();

signals:

private:
    Ui::SleepMonitor *ui;

};
#endif // SLEEPMONITOR_H
