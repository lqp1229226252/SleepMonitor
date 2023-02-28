#ifndef SLEEPINTERVENTIONWIDGET_H
#define SLEEPINTERVENTIONWIDGET_H

#include "commsetwidget.h"
#include <QWidget>
namespace Ui {
class SleepInterventionWidget;
}

class SleepInterventionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SleepInterventionWidget(QWidget *parent = nullptr);
    ~SleepInterventionWidget();
    void init_buf();
    void read_data();
    void cal_verify();
    QByteArray enCode();
    void get_commSetWidget(CommSetWidget *);

signals:
    void sendSignal(QByteArray);
private slots:
    void on_send_clicked();

private:
    Ui::SleepInterventionWidget *ui;
    uint8_t buf_head;
    uint16_t buf_len;
    uint8_t  buf_code;
    uint8_t  buf_amplitude_data;
    uint8_t  buf_timedata;
    uint8_t  buf_verify;
    uint8_t  buf_num;
    uint8_t  buf_end;
    CommSetWidget *commSetWidget;
};

#endif // SLEEPINTERVENTIONWIDGET_H
