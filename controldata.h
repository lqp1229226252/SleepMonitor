#ifndef CONTROLDATA_H
#define CONTROLDATA_H
// #include "QObject"
#include "struct_data.h"
class ControlData
{
private:
    /* data */
    CTRL_DATA m_CtrlData;
    SD_AMOUNT m_SensorDataAmount;
public:
    ControlData(/* args */);
    ~ControlData();
    void setCtrlData(quint8,quint8,quint16,quint16);
    void setSensorDataAmount(int,int,int,int,qint16);
    CTRL_DATA getCtrlData();
    CTRL_DATA* getCtrlDate();
    SD_AMOUNT getSensorDataAmount();
    int getFPAmount();
    int getSnoreAmount();
    int getLightAmount();
    int getGroAccAmount();
    qint16 getFrameLen();

};
#endif
