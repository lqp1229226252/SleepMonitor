#include "src/sleepmonitor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SleepMonitor w;
    w.setWindowIcon(QIcon("./icon/sleepmonitor.ico"));
    w.show();
    return a.exec();
}
