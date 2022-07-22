#ifndef COMMSETWND_H
#define COMMSETWND_H

#include <QWidget>
#include "QtSerialPort/QSerialPort"
#include "sensordata.h"
#include "filepath.h"
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
    void dataSignal(QVector<double>);
    void stataSignal(QByteArray);
    void lossRateChange(float);
    void fileStorageState(bool);
    void FilePathChange(QStringList);
    void FilePathChange(int,QString);
private slots:
    void on_start_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_serial_detect_clicked();

    void readData();
    void dealError(QSerialPort::SerialPortError);

    void dataSlot(QVector<double>);
    void stataSlot(QByteArray);
    void lossRateChangeSlot(float);

    void on_file_set_clicked();

    void on_file_start_clicked();

    void on_file_end_clicked();
    void updatafilePath();
    void updatefilePath(int,QString);

private:
    Ui::CommSetWidget *ui;
    QFont font;//fontawsome图标
    QSerialPort *serial;
    QStringList serialnames;
    bool pause_flag;//暂停标志位
    SensorData sensordata;
    FilePath *filepath;
};

#endif // COMMSETWND_H
