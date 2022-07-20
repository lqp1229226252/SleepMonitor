#include "ControlWnd.h"
#include "ui_ControlWnd.h"
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>

ControlWnd::ControlWnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

    ui->setupUi(this);
    setWindowTitle(tr("控制窗口"));//设置标题

}




ControlWnd::~ControlWnd()
{
    delete ui;
}

