#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>
#include "struct_data.h"
#include "QVector"
#include "iostream"
using namespace std;
typedef union data32
{
    qint32 data;
    char buffer[4];

}Data32;
typedef union data16
{
    qint16 data;
    char buffer[4];
}Data16;
class SensorData : public QObject
{
    Q_OBJECT
public:
    explicit SensorData(QObject *parent = nullptr);
    void appendData(QByteArray pData);
    void setCtrlData(int,int,int,int,quint16);
    int getFPNum();
    int getLightNum();
    int getSnoreNum();
    int getGroAccNum();

    QVector<SD_FP> getFP1();
    QVector<SD_FP> getFP2();

    QVector<SD_LIGTH> getRedLight();
    QVector<SD_LIGTH> getNearReadLight();
    QVector<SD_LIGTH> getGreenLight();

    QVector<SD_SNORE> getSnoreLeft();
    QVector<SD_SNORE> getSnoreRight();

    QVector<double>   getRollAngel();
    QVector<double>   getSeatAngle();
    QVector<double>   getActionAcc();

    QVector<SD_GRO>   getGrox();
    QVector<SD_GRO>   getGroy();
    QVector<SD_GRO>   getGroz();

    QVector<SD_ACC>   getAccx();
    QVector<SD_ACC>   getAccy();
    QVector<SD_ACC>   getAccz();
signals:
    void dataSignal(QByteArray);
    void dataSignal(QVector<double>);
    void stataSignal(QByteArray);
    void stataSignal(STATE_DATA);
    void lossRateChange(float);

    void FP1OverFlow();
    void FP2OverFlow();

    void redLightOverFlow();
    void nearRedLightOverFlow();
    void greenLightOverFlow();

    void seatAngleOverFlow();
    void rollAngleOverFlow();
    void accOverFlow();

    void lSnoreOverFlow();
    void rSnoreOverFlow();

    void GroXOverFlow();
    void GroYOverFlow();
    void GroZOverFlow();
    void ACCXOverFlow();
    void ACCYOverFlow();
    void ACCZOverFlow();

public slots:
private:
    /* data */
   QVector<qint8> m_vSrcData;// 接收到的原始数据
   QVector<SD_FP> m_vFP1, m_vFP2;
   int m_nFP1Amount,m_nFP2Amount; // FP1 数据量
   QVector<SD_LIGTH> m_vRedLight, m_vNearRedLight, m_vGreenLight;
   int m_nRedLightAmount,m_nNearRedLightAmount,m_nGreenLightAmount; // 3 种光的数据量
   QVector<double> m_vSeatAngle; // 坐立角
   QVector<double> m_vRollAngle; // 翻滚角
   QVector<double> m_vAcceleration; // 运动加速度
   int m_nSeatAngleAmount,m_nRollAngleAmount,m_nAccAmount; // 2 种角和运动加速度的数据量
   QVector<SD_SNORE> m_vLSnore, m_vRSnore;
   int m_nLSnoreAmount,m_nRSnoreAmount; // 鼾声数据量
   QVector<SD_GRO> m_vXgro, m_vYgro, m_vZgro;
   QVector<SD_ACC> m_vXacc, m_vYacc, m_vZacc;
   int m_nGroxAmount,m_nGroyAmount,m_nGrozAmount,m_nAccxAmount,m_nAccyAmount,m_nAcczAmount; // gro、acc 数据量
   QVector<STATE_DATA> m_vStateData; // 状态数据

   int m_nFP;  //数据帧中FP的数量
   int m_nLight; //数据帧中Light的数量
   int m_nSnore; //数据帧中Snore的数量
   int m_nGroAcc;//数据帧中GorACC的数量
   quint16 m_source; //采集源


   void catData(QByteArray pData);//将数据连接到m_vSrcData
   bool findBufHead();//查找帧头，帧头之前的数据丢弃,查找成功返回Ture,失败返回False
   bool findBufEnd();//查找帧尾,查找成功返回True,失败返回False
   QByteArray getBuffer();//获取一帧数据
   QByteArray getdata(QByteArray,int,int);//根据输入的开始和结束获取buffer中的数据

   void paraData(QByteArray buffer);//解析数据
   void paraAllData(QByteArray buffer);//解析睡眠贴全部数据
   SD_FP paraFPData(QByteArray buffer);//解析FP数据
   SD_LIGTH paraLightData(QByteArray buffer);//解析红光/近红光/绿光数据
   double paraAngleData(QByteArray buffer);//解析角度数据
   double paraActionAccData(QByteArray buffer);//解析运动加速度
   SD_SNORE paraSnoreData(QByteArray buffer);//解析鼾声数据
   SD_ACC paraAccData(QByteArray buffer);//解析ACC数据
   SD_GRO paraGroData(QByteArray buffer);//解析Gro数据



   STATE_DATA paraStateData(QByteArray buffer); //解析状态数据

   void insertData(QVector<SD_FP> &list,SD_FP data,int);
   void insertData(QVector<SD_LIGTH> &list,SD_LIGTH data,int);
   void insertData(QVector<double> &list,double data,int);
   void insertData(QVector<SD_SNORE> &list,SD_SNORE data,int);
   void insertData(QVector<SD_GRO> &list,SD_GRO data,int);
   void insertData(QVector<SD_ACC> &list,SD_ACC data,int);
   void insertData(QVector<STATE_DATA> &list,STATE_DATA data);

   void updateFP1Amount();
   void updateFP2Amount();

   void updateRedLightAmount();
   void updateNearRedLightAmount();
   void updateGreenLightAmount();

   void updateSeatAngelAmount();
   void updateRollAngelAmount();
   void updateAccAmount();

   void updateLSnoreAmount();
   void updateRSnoreAmount();


   void updateGROXAmount();
   void updateGROYAmount();
   void updateGROZAmount();
   void updateACCXAmount();
   void updateACCYAmount();
   void updateACCZAmount();

   QByteArray getSendData(char *buffer);

   void calPacketLossRate(int num);
};

#endif // SENSORDATA_H
