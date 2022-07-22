#include "controldata.h"
#include "QFileInfo"
#include "QDebug"
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
    storage_falg=false;
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

bool ControlData::getSaveFlag()
{
    return this->storage_falg;
}

void ControlData::SaveBegin(QStringList paths)
{
    this->m_strPathFP=paths[0];
    this->m_strPathLight=paths[1];
    this->m_strPathAngleAcc=paths[2];
    this->m_strPathSnore=paths[3];
    this->m_strPathGroAcc=paths[4];
    for(int i=0;i<paths.length();i++)
    {
        QFileInfo info(paths[i]);
        if(info.isFile())
        {
            QFile file(paths[i]);
            file.open(QIODevice::Truncate);
            file.close();
        }
        else {
            QFile file(paths[i]);
            file.open(QIODevice::WriteOnly);
            file.close();
        }
    }
    this->storage_falg=true;
}

void ControlData::SaveEnd()
{
    this->storage_falg=false;
}

void ControlData::saveFP(QVector<SD_FP> fp,char flag)
{
    QFile file(m_strPathFP);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray array;
        array.append(flag);
        for(int i=0;i<fp.length();i++)
        {
            array.append(QString::number(fp[i].dbVal));
        }
    }
    else {
        qDebug()<<"文件打开失败";
    }
}

void ControlData::saveLight(QVector<SD_LIGTH> light,char flag)
{
    QFile file(m_strPathLight);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray array;
        array.append(flag);
        for(int i=0;i<light.length();i++)
        {
            array.append(QString::number(light[i].dbVal));
        }
    }
    else {
        qDebug()<<"文件打开失败";
    }
}

void ControlData::saveAngleAcc(QVector<double> angle, char flag)
{
    QFile file(m_strPathAngleAcc);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray array;
        array.append(flag);
        for(int i=0;i<angle.length();i++)
        {
            array.append(QString::number(angle[i]));
        }
    }
    else {
        qDebug()<<"文件打开失败";
    }
}

void ControlData::saveSnore(QVector<SD_SNORE> snore)
{
    QFile file(m_strPathSnore);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray array;
        for(int i=0;i<snore.length();i++)
        {
            array.append(QString::number(snore[i].dbVal));
        }
    }
    else {
        qDebug()<<"文件打开失败";
    }
}

void ControlData::saveGro(QVector<SD_GRO> gro, char flag)
{
    QFile file(m_strPathGroAcc);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray array;
        array.append(flag);
        for(int i=0;i<gro.length();i++)
        {
            array.append(QString::number(gro[i].dbVal));
        }
    }
    else {
        qDebug()<<"文件打开失败";
    }
}

void ControlData::saveACC(QVector<SD_ACC> acc, char flag)
{
    QFile file(m_strPathGroAcc);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        QByteArray array;
        array.append(flag);
        for(int i=0;i<acc.length();i++)
        {
            array.append(QString::number(acc[i].dbVal));
        }
    }
    else {
        qDebug()<<"文件打开失败";
    }
}
