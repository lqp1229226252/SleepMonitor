#include "controldata.h"

ControlData::ControlData(/* args */)
{
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
CTRL_DATA* ControlData::getCtrlData()
{
    return &m_CtrlData;
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