#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QComboBox"
#include "QLabel"
#include "QVBoxLayout"
#include "QCheckBox"
#include "QButtonGroup"
#include "QLineEdit"
#include "QPushButton"
#include "QtSerialPort/QSerialPort"
class ComboBox: public QWidget
{
    Q_OBJECT
public:
    ComboBox(QWidget *parent=NULL);
    ~ComboBox();
    void setlabel(QString Text);
    void setItems(QStringList list);
    void setFontSize(QFont font);
    void setSize(int w=100,int h=80);
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged(int index);

public slots:
    void slot(int index);

private:
    QLabel label;
    QComboBox combobox;
    QVBoxLayout *vbox;

};
class CheckBoxList:public QWidget
{
    Q_OBJECT
public:
    CheckBoxList(QWidget *parent=NULL);
    ~CheckBoxList();
    void create(QStringList labels,bool flag=false);
    void setFontSize(QFont font);
signals:
    void check(QByteArray);
public slots:
    void slot(int index);
private:
    QButtonGroup groupbutton;
    QList<QLabel*>  labellist;
    QList<QLabel*>  labelempty;//用于填补空白
    QList<QCheckBox*> checkboxlist;
    QGridLayout    *grid;
    int length;
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void initBuf();
    void setUI();
    void setConnect();
    uint8_t cal_verify(uint16_t);
    void setFontSize(int size);
    void setSize(int w,int h);
signals:
    void sendSingal(QByteArray);
public slots:
    void comboBoxSlot1(int index);
    void comboBoxSlot2(int index);
    void comboBoxSlot3(int index);
    void comboBoxSlot4(int index);
    void comboBoxSlot5(int index);
    void checkSlot1(QByteArray buffer);
    void checkSlot2(QByteArray buffer);
    void sendSlot();
private:
    ComboBox collect_control_ui;
    ComboBox brain_collect_rate_ui;
    ComboBox snoring_collect_rate_ui;
    ComboBox brainoxygen_collect_rate_ui;
    ComboBox body_action_rate_ui;
    QHBoxLayout *hbox1;

    //采集数据源源
    QLabel collect_label;
    CheckBoxList collect1;
    CheckBoxList collect2;
    QVBoxLayout *collect_vbox;

   //采集点数
    QVBoxLayout *point_vbox;
    QLabel collect_point_label;
    QLineEdit collect_point;

    QHBoxLayout *hbox2;
    QVBoxLayout *vbox1;
    //发送
    QVBoxLayout *send_vbox;
    QPushButton send_btn;
    QHBoxLayout *hbox;

    //控制帧
    uint8_t buf_head;
    uint16_t  buf_len;
    uint8_t   buf_code;
    uint8_t   buf_collect_contral;
    uint8_t   buf_collect_rate;
    uint8_t   buf_collect_res_high;
    uint8_t   buf_collect_res_low;
    uint8_t   buf_num;
    uint8_t   buf_end;

    QSerialPort *serial;


};

#endif // WIDGET_H
