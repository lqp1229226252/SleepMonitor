#include "controldata.h"

ControlData::ControlData(/* args */)
{
    this->m_CtrlData.nCtrl=0xAA;
    this->m_CtrlData.nRate=0xA0;
    this->m_CtrlData.nSrc=0;
    this->m_CtrlData.nAmont=1000;
    this->m_SensorDataAmount.nFP=5;
    this->m_SensorDataAmount.nSnore=10;
    this->m_SensorDataAmount.nLight=1;
    this->m_SensorDataAmount.nGroAcc=1;
}

ControlData::~ControlData()
{
}
void ControlData::setCtrlData(quint8 Ctrl,quint8 Rate,quint16 Src,quint16 Amont)
{
    this->m_CtrlData.nCtrl=Ctrl;
    this->m_CtrlData.nRate=Rate;
    this->m_CtrlData.nSrc=Src;
    this->m_CtrlData.nAmont=Amont;
}
void ControlData::setSensorDataAmount(int FP,int Snore,int Light,int GroAcc,qint16 FrameLen)
{
    this->m_SensorDataAmount.nFP=FP;
    this->m_SensorDataAmount.nSnore=Snore;
    this->m_SensorDataAmount.nLight=Light;
    this->m_SensorDataAmount.nGroAcc=GroAcc;
    this->m_SensorDataAmount.nFrameLen=FrameLen;
}
CTRL_DATA ControlData::getCtrlData()
{
    return this->m_CtrlData;
}
CTRL_DATA* ControlData::getCtrlDate()
{
    return &m_CtrlData;
}

SD_AMOUNT ControlData::getSensorDataAmount()
{
    return this->m_SensorDataAmount;
}

int ControlData::getFPAmount()
{
    return this->m_SensorDataAmount.nFP;
}
int ControlData::getSnoreAmount()
{
    return this->m_SensorDataAmount.nSnore;
}
int ControlData::getLightAmount()
{
    return this->m_SensorDataAmount.nLight;
}
int ControlData::getGroAccAmount()
{
    return this->m_SensorDataAmount.nGroAcc;
}
qint16 ControlData::getFrameLen()
{
    return this->m_SensorDataAmount.nFrameLen;
}
