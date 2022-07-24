#include "controldata.h"
#include "QFileInfo"
#include "QDebug"
ControlData::ControlData(QObject *object):QObject (object)
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

//ControlData::~ControlData()
//{
//}
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

void ControlData::checkFileExit(QString path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.close();
}

void ControlData::SaveBegin(QString prefix)
{
    //FP
    m_strPathFP1=prefix+"_FP1.csv";
    checkFileExit(m_strPathFP1);
    m_strPathFP2=prefix+"_FP2.csv";
    checkFileExit(m_strPathFP2);
    //灯光
    m_strPathRedLight=prefix+"_redlight.csv";
    checkFileExit(m_strPathRedLight);
    m_strPathRedNearLight=prefix+"_rednearlight.csv";
    checkFileExit(m_strPathRedNearLight);
    m_strPathGreenLight=prefix+"_greenlight.csv";
    checkFileExit(m_strPathGreenLight);
    //鼾声
    m_strPathLSnore=prefix+"_lsnore.csv";
    checkFileExit(m_strPathLSnore);
    m_strPathRSnore=prefix+"_rsnore.csv";
    checkFileExit(m_strPathRSnore);
    //角度和加速度
    m_strPathSeatAngle=prefix+"_seatangle.csv";
    checkFileExit(m_strPathSeatAngle);
    m_strPathRollAngle=prefix+"_rollangel.csv";
    checkFileExit(m_strPathRollAngle);
    m_strPathACC=prefix+"_acc.csv";
    checkFileExit(m_strPathACC);
    //gro
    m_strPathGroX=prefix+"_grox.csv";
    checkFileExit(m_strPathGroX);
    m_strPathGroY=prefix+"_groy.csv";
    checkFileExit(m_strPathGroY);
    m_strPathGroZ=prefix+"_groz.csv";
    checkFileExit(m_strPathGroZ);
    //acc
    m_strPathAccX=prefix+"_accx.csv";
    checkFileExit(m_strPathAccX);
    m_strPathAccY=prefix+"_accy.csv";
    checkFileExit(m_strPathAccY);
    m_strPathAccZ=prefix+"_accz,csv";
    checkFileExit(m_strPathAccZ);

    this->storage_falg=true;
}

void ControlData::SaveEnd()
{
    this->storage_falg=false;
}

void ControlData::saveFP(QString path, QVector<SD_FP> data)
{
    QString str="";
    for (int i=0;i<data.length();i++) {
        str.append(QString::number(data[i].dbVal));
        str.append(",");
    }
    str[str.length()-1]='\n';
    QFile file(path);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str.toStdString().c_str());
    }
}

void ControlData::saveLight(QString path, QVector<SD_LIGTH> data)
{
    QString str="";
    for (int i=0;i<data.length();i++) {
        str.append(QString::number(data[i].dbVal));
        str.append(",");
    }
    str[data.length()-1]='\n';
    QFile file(path);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str.toStdString().c_str());
    }
}

void ControlData::saveAngleAcc(QString path, QVector<double> data)
{
    QString str="";
    for (int i=0;i<data.length();i++) {
        str.append(QString::number(data[i]));
        str.append(",");
    }
    str[data.length()-1]='\n';
    QFile file(path);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str.toStdString().c_str());
    }
}

void ControlData::saveSnore(QString path, QVector<SD_SNORE> data)
{
    QString str="";
    for (int i=0;i<data.length();i++) {
        str.append(QString::number(data[i].dbVal));
        str.append(",");
    }
    str[data.length()-1]='\n';
    QFile file(path);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str.toStdString().c_str());
    }
}

void ControlData::saveGro(QString path, QVector<SD_GRO> data)
{
    QString str="";
    for (int i=0;i<data.length();i++) {
        str.append(QString::number(data[i].dbVal));
        str.append(",");
    }
    str[data.length()-1]='\n';
    QFile file(path);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str.toStdString().c_str());
    }
}

void ControlData::saveACC(QString path, QVector<SD_ACC> data)
{
    QString str="";
    for (int i=0;i<data.length();i++) {
        str.append(QString::number(data[i].dbVal));
        str.append(",");
    }
    str[data.length()-1]='\n';
    QFile file(path);
    bool isok=file.open(QIODevice::Append);
    if(isok)
    {
        file.write(str.toStdString().c_str());
    }
}

void ControlData::saveFP1(QVector<SD_FP> data)
{
    saveFP(m_strPathFP1,data);
}

void ControlData::saveFP2(QVector<SD_FP> data)
{
    saveFP(m_strPathFP2,data);
}

void ControlData::saveRedLight(QVector<SD_LIGTH> data)
{
    saveLight(m_strPathRedLight,data);
}

void ControlData::saveRedNearLight(QVector<SD_LIGTH> data)
{
    saveLight(m_strPathRedNearLight,data);
}

void ControlData::saveGreenLight(QVector<SD_LIGTH> data)
{
    saveLight(m_strPathGreenLight,data);
}

void ControlData::saveLSnore(QVector<SD_SNORE> data)
{
    saveSnore(m_strPathLSnore,data);
}

void ControlData::saveRSnore(QVector<SD_SNORE> data)
{
    saveSnore(m_strPathRSnore,data);
}

void ControlData::saveSeatAngle(QVector<double> data)
{
    saveAngleAcc(m_strPathSeatAngle,data);
}

void ControlData::saveRollAngel(QVector<double> data)
{
    saveAngleAcc(m_strPathRollAngle,data);
}

void ControlData::saveAcc(QVector<double> data)
{
    saveAngleAcc(m_strPathACC,data);
}

void ControlData::saveGrox(QVector<SD_GRO> data)
{
    saveGro(m_strPathGroX,data);
}

void ControlData::saveGroy(QVector<SD_GRO> data)
{
    saveGro(m_strPathGroY,data);
}

void ControlData::saveGroz(QVector<SD_GRO> data)
{
    saveGro(m_strPathGroZ,data);
}

void ControlData::saveACCx(QVector<SD_ACC> data)
{
    saveACC(m_strPathAccX,data);
}

void ControlData::saveACCy(QVector<SD_ACC> data)
{
    saveACC(m_strPathAccY,data);
}

void ControlData::saveAccz(QVector<SD_ACC> data)
{
  saveACC(m_strPathAccZ,data);
}
