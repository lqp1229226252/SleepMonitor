#ifndef COMMSETWND_H
#define COMMSETWND_H

#include <QWidget>
#include "QtSerialPort/QSerialPort"
#include "sensordata.h"
namespace Ui {
class CommSetWidget;
}

class CommSetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CommSetWidget(QWidget *parent = nullptr);
    ~CommSetWidget();
    void setFont();
    void setUI();
    void setLightColor(Qt::GlobalColor);
    void setSize(int w,int h);
    void write(QByteArray);
    void messageBox(QString str);
    void setCtrlData(int,int,int,int);
signals:
    void dataSignal(QByteArray);
    void stataSignal(QByteArray);
    void lossRateChange(float);
private slots:
    void on_start_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_serial_detect_clicked();

    void readData();
    void dealError(QSerialPort::SerialPortError);

    void dataSlot(QByteArray);
    void stataSlot(QByteArray);
    void lossRateChangeSlot(float);

private:
    Ui::CommSetWidget *ui;
    QFont font;//fontawsome图标
    QSerialPort *serial;
    QStringList serialnames;
    bool pause_flag;//暂停标志位
    SensorData sensordata;

};

#endif // COMMSETWND_H
