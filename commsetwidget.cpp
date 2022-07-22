#include "commsetwidget.h"
#include "ui_commsetwidget.h"
#include "QFontDatabase"
#include "QDebug"
#include "QtSerialPort/QSerialPortInfo"
#include "QLineEdit"
#include <QPalette>
#include "QMessageBox"
#include "filepath.h"
CommSetWidget::CommSetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommSetWidget)
{
    ui->setupUi(this);
    this->serial=new QSerialPort();
    filepath=new FilePath();
    this->pause_flag=false;
    setFont();
    setUI();
    on_serial_detect_clicked();
    connect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(serial,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(dealError(QSerialPort::SerialPortError)));
    //sensordata的信号
    connect(&sensordata,SIGNAL(dataSignal(QByteArray)),this,SLOT(dataSlot(QByteArray)));
    connect(&sensordata,SIGNAL(stataSignal(QByteArray)),this,SLOT(stataSlot(QByteArray)));
    connect(&sensordata,SIGNAL(dataSignal(QVector<double>)),this,SLOT(dataSlot(QVector<double>)));
    connect(&sensordata,SIGNAL(stataSignal(STATE_DATA)),this,SLOT(stataSlot(STATE_DATA)));
    connect(&sensordata,SIGNAL(lossRateChange(float)),this,SLOT(lossRateChangeSlot(float)));
    //文件路径设置窗口信号
    connect(filepath,SIGNAL(close()),this,SLOT(updatafilePath()));
    connect(filepath,SIGNAL(PathChang(int,QString)),this,SLOT(updatefilePath(int,QString)));
}

CommSetWidget::~CommSetWidget()
{
    delete ui;
    delete serial;
    delete filepath;
}
void CommSetWidget::setFont()
{
    int id=QFontDatabase::addApplicationFont(":/fontawesome-webfont.ttf");
    QString fontname=QFontDatabase::applicationFontFamilies(id).at(0);
    font.setFamily(fontname);
    ui->stop->setFont(font);
    ui->start->setFont(font);
    ui->pause->setFont(font);
    ui->direct_light->setFont(font);
}
void CommSetWidget::setUI()
{

    this->setFixedSize(600,200);

    ui->direct_light->setText(QChar(0xf111));
    this->setLightColor(Qt::red);
    ui->direct_light->setFixedSize(20,20);
    ui->start->setText(QChar(0xf04b));
    ui->pause->setText(QChar(0xf04c));
    ui->stop->setText(QChar(0xf04d));

    ui->start->setStyleSheet("QPushButton { background-color: transparent;border:none; }");
    ui->pause->setStyleSheet("QPushButton { background-color: transparent;border:none; }");
    ui->stop->setStyleSheet("QPushButton { background-color: transparent;border:none; }");

    ui->start->setFixedSize(20,20);
    ui->pause->setFixedSize(20,20);
    ui->stop->setFixedSize(20,20);

    QStringList bound_rates={"1000000","9600","57600","115200"};
    ui->bound_rate->addItems(bound_rates);
    ui->bound_rate->setEditable(true);
    ui->bound_rate->setFixedSize(100,20);


    ui->serial->setFixedSize(100,20);
    ui->serial_detect->setFixedSize(100,20);
}

void CommSetWidget::setLightColor(Qt::GlobalColor color)
{
    QPalette pe;
    pe.setColor(QPalette::Background,Qt::black);
    pe.setColor(QPalette::WindowText,color);
    ui->direct_light->setPalette(pe);
}
void CommSetWidget::setSize(int w, int h)
{

}
void CommSetWidget::write(QByteArray buffer)
{
    this->serial->write(buffer);
}
void CommSetWidget::messageBox(QString str)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("提示");
    msgBox.setText(str);
    msgBox.exec();
}

void CommSetWidget::setCtrlData(int nFP, int nSnore, int nLight, int nGroAcc)
{
    this->sensordata.setCtrlData(nFP,nSnore,nLight,nGroAcc);
}

QStringList CommSetWidget::getAllPath()
{
    QStringList list;
    list.append(filepath->getFPPath());
    list.append(filepath->getLightPath());
    list.append(filepath->getAngleAccPath());
    list.append(filepath->getSnorePath());
    list.append(filepath->getGroAccPath());
    return list;
}
void CommSetWidget::on_start_clicked()
{
    QString portname=ui->serial->currentText();
    int bound_rate=ui->bound_rate->currentText().toInt();
    serial->setPortName(portname);
    serial->setBaudRate(bound_rate);
    if(!serial->isOpen())
    {
        if(serial->open(QIODevice::ReadWrite))
        {
            this->pause_flag=false;
            this->setLightColor(Qt::yellow);
        }
        else {
            this->messageBox("串口打开失败");
        }
    }
    else {
        this->messageBox("串口已经打开");
    }

}

void CommSetWidget::on_pause_clicked()
{
    write("123456");
    if(serial->isOpen())
    {
       pause_flag=true;
    }
    else {
        this->messageBox("串口没有打开");
    }
}

void CommSetWidget::on_stop_clicked()
{
    if(serial->isOpen())
    {
        this->serial->close();
        this->setLightColor(Qt::red);
        this->pause_flag=false;
    }
    else {
      this->messageBox("串口没有打开");
    }
}

void CommSetWidget::on_serial_detect_clicked()
{
    ui->serial->clear();
    this->serialnames.clear();
    //获取全部串口信息
    QList<QSerialPortInfo> infos=QSerialPortInfo::availablePorts();
    //将检测的串口名字插入serialnames
    for(int i=0;i<infos.size();i++)
    {
       this->serialnames.append(infos[i].portName());
    }
    //将检测的串口显示到窗口
    ui->serial->addItems(this->serialnames);
}
void CommSetWidget::readData()
{
    this->setLightColor(Qt::green);
    if(!this->pause_flag)
    {
        //开始读取数据
        QByteArray data=this->serial->readAll();
//        qDebug()<<data;
        sensordata.appendData(data);


    }
}

void CommSetWidget::dealError(QSerialPort::SerialPortError error)
{
    if(error==QSerialPort::ResourceError)
    {
        this->setLightColor(Qt::red);
        this->messageBox("设备已经断开");
    }
}

void CommSetWidget::dataSlot(QByteArray data)
{
//    qDebug()<<data;
    emit(dataSignal(data));
}

void CommSetWidget::stataSlot(QByteArray data)
{
//    qDebug()<<data;
    emit(stataSignal(data));
}

void CommSetWidget::dataSlot(QVector<double> data)
{
//    qDebug()<<data;
    emit(dataSignal(data));
}

void CommSetWidget::stataSlot(STATE_DATA data)
{
    emit(stataSignal(data));
}

void CommSetWidget::lossRateChangeSlot(float loss)
{
//    qDebug()<<loss;
    emit(lossRateChange(loss));
}

void CommSetWidget::on_file_set_clicked()
{
    filepath->show();
}

void CommSetWidget::on_file_start_clicked()
{
    emit(fileStorageState(true));
}

void CommSetWidget::on_file_end_clicked()
{
    emit(fileStorageState(true));
}

void CommSetWidget::updatafilePath()
{
    QStringList list;
    list.append(filepath->getFPPath());
    list.append(filepath->getLightPath());
    list.append(filepath->getAngleAccPath());
    list.append(filepath->getSnorePath());
    list.append(filepath->getGroAccPath());
//    qDebug()<<list;
    emit(FilePathChange(list));
}

void CommSetWidget::updatefilePath(int type, QString Path)
{
    emit(FilePathChange(type,Path));
}
