#include "sleepmonitor.h"
#include "eegwnd.h"
#include <QApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SleepMonitor w;
//    w.setWindowFlag(Qt::Window);
//    w.showFullScreen();
    w.setWindowState(Qt::WindowMaximized);
    w.setWindowIcon(QIcon(":/icon/sleepmonitor.ico"));
    w.show();
//    test t;
//    t.show();

    return a.exec();
}
