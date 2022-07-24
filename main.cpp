#include "sleepmonitor.h"
#include "eegwnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SleepMonitor w;
    w.setWindowFlag(Qt::Window);
    w.showFullScreen();
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
