#include "sleepinterventionwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SleepInterventionWidget w;
    w.show();

    return a.exec();
}
