#include "ContralDataWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ContralDataWidget w;
    w.show();

    return a.exec();
}
