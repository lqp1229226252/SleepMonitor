#ifndef COMMSETWND_H
#define COMMSETWND_H

#include <QWidget>
#include "QtSerialPort/QSerialPort"
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
    void write(QByteArray);
    void messageBox(QString str);
private slots:
    void on_start_clicked();

    void on_pause_clicked();

    void on_stop_clicked();

    void on_serial_detect_clicked();

    void readData();
    void dealError(QSerialPort::SerialPortError);

private:
    Ui::CommSetWidget *ui;
    QFont font;//fontawsome图标
    QSerialPort *serial;
    QStringList serialnames;
    bool pause_flag;//暂停标志位
};

#endif // COMMSETWND_H
