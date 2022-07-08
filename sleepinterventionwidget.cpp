#include "sleepinterventionwidget.h"
#include "ui_sleepinterventionwidget.h"
#include"QDebug"
SleepInterventionWidget::SleepInterventionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SleepInterventionWidget)
{
    ui->setupUi(this);
    init_buf();
}

SleepInterventionWidget::~SleepInterventionWidget()
{
    delete ui;
}

void SleepInterventionWidget::init_buf()
{
    this->buf_head=0xBE;
    this->buf_len=0x09;
    this->buf_code=0x03;
    this->buf_amplitude_data=0x0A;
    this->buf_timedata=0x0A;
    this->buf_num=0;
    this->buf_end=0xEB;
}

void SleepInterventionWidget::read_data()
{
    this->buf_amplitude_data=uint8_t(ui->amplitude->value());
    this->buf_timedata=uint8_t(ui->time->value());
}

void SleepInterventionWidget::cal_verify()
{
    int sum=this->buf_head;
    sum+=this->buf_len;
    sum+=this->buf_code;
    sum+=this->buf_amplitude_data;
    sum+=this->buf_timedata;
    this->buf_verify=uint8_t(sum);
}

QByteArray SleepInterventionWidget::enCode()
{

     QByteArray buffer;

     buffer.append(char(buf_head));
     buffer.append(char(buf_len));
     buffer.append(char(buf_len>>8));
     buffer.append(char(buf_code));
     buffer.append(char(buf_amplitude_data));
     buffer.append(char(buf_timedata));
     buffer.append(char(buf_verify));
     buffer.append(char(buf_num));
     buffer.append(char(buf_end));
     return buffer;
}

void SleepInterventionWidget::on_send_clicked()
{
    read_data();
    cal_verify();
    this->buf_num++;
    QByteArray buffer=enCode();
//    qDebug()<<buffer;
    emit(sendSignal(buffer));
}
