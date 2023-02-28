#include "QApplication"
#include "curvegroup.h"
int main(int args,char **argv)
{
    QApplication app(args,argv);
    CurveGroup w;
    w.resize(640,480);
    w.setMaxPoint(100);
    w.setMinMax(-3,3);
    w.setCurveNumGroup(5);
    w.setCurveNum(10);
    w.setCurveLabels(QStringList({ "FP","AF","Pf","PF","PG","FP1","AF1","Pf1","PF1","PG1"}));
    w.next();
    w.test_chart();
    w.show();

    return app.exec();
}
