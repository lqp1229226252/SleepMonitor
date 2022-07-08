#include "widget.h"
#include "ui_widget.h"
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    int color=0;
    ui->setupUi(this);
    setWindowTitle(tr("控制窗口"));//设置标题
    QString background = "background-color:";
    switch(color){
    case 0:
        //红色
        background +="rgb(255,0,0);";
        break;
    case 1:
        // 黄色
        background += "rgb(255,255,0);";
        break;
    case 2:
        // 绿色
        background += "rgb(0,255,0);";
        break;
    default:
            break;
    }
    QLabel *led0=ui->led;
    led0->setText("");
    led0->setGeometry(162,72,35,35);
    led0->setStyleSheet("border:4px groove gray;border-radius:10px;padding:2px 4px;"+background);
    QStringList list;
    list<<"1000000"<<"900000";
    ui->comboBox_2->addItems(list);
    //ui->comboBox->setEditable(true);
    ui->comboBox_2->setEditable(true);
    connect(ui->searchbutton,&QPushButton::clicked,this,[=](){
        search_button();
    });
}

void Widget::search_button(){
    ui->comboBox->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
           ui->comboBox->addItem(info.portName());
    }
}


Widget::~Widget()
{
    delete ui;
}

