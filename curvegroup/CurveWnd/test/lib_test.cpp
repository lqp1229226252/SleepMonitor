#include "curvewnd.h"
#include "QApplication"
int main(int args,char **argv)
{
    QApplication app(args,argv);
    CurveWnd curvewnd;
    //����ͼ���С
    curvewnd.resize(640,480);
    //���ò�����
    curvewnd.setAxisXRange(0,200);
    //����������Ŀ
    curvewnd.setMinMax(-3,3);
    curvewnd.setCurveNum(5);
    //�������߱�ǩ
    curvewnd.setCurveLabels(QStringList({ "FP","AF","Pf","PF","PG"}));
    curvewnd.show();
    //��������
    curvewnd.test_chart();
    return app.exec();
}
