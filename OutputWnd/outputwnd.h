#ifndef OUTPUTWND_H
#define OUTPUTWND_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class OutPutWnd; }
QT_END_NAMESPACE

class OutPutWnd : public QWidget
{
    Q_OBJECT

public:
    OutPutWnd(QWidget *parent = nullptr);
    ~OutPutWnd();

    void OutPut(QString str);

private:
    Ui::OutPutWnd *ui;
};
#endif // OUTPUTWND_H
