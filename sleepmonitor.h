#ifndef SLEEPMONITOR_H
#define SLEEPMONITOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SleepMonitor; }
QT_END_NAMESPACE

class SleepMonitor : public QMainWindow
{
    Q_OBJECT

public:
    SleepMonitor(QWidget *parent = nullptr);
    ~SleepMonitor();

private:
    Ui::SleepMonitor *ui;
};
#endif // SLEEPMONITOR_H
