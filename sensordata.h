#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <QObject>
#include "struct_data.h"
#include "QVector"
class SensorData : public QObject
{
    Q_OBJECT
public:
    explicit SensorData(QObject *parent = nullptr);
    void appendData(QByteArray pData);
    void setCtrlData(int,int,int,int);
    int getFPNum();
    int getLightNum();
    int getSnoreNum();
    int getGroAccNum();
signals:
    void dataSignal(QByteArray);
    void stataSignal(QByteArray);
    void lossRateChange(float);
public slots:
private:
    /* data */
   QVector<qint8> m_vSrcData;// 接收到的原始数据
   QVector<SD_FP> m_vFP1, m_vFP2;
   int m_nFPAmount; // FP 数据量
   QVector<SD_LIGTH> m_vRedLight, m_vNearRedLight, m_vGreenLight;
   int m_nLightAmount; // 3 种光的数据量
   QVector<double> m_vSeatAngle; // 坐立角
   QVector<double> m_vRollAngle; // 翻滚角
   QVector<double> m_vAcceleration; // 运动加速度
   int m_nAngleAccAmount; // 2 种角和运动加速度的数据量
   QVector<SD_SNORE> m_vLSnore, m_vRSnore;
   int m_nSnoreAmount; // 鼾声数据量
   QVector<SD_GRO> m_vXgro, m_vYgro, m_vZgro;
   QVector<SD_ACC> m_vXacc, m_vYacc, m_vZacc;
   int m_nGroAccAmount; // gro、acc 数据量
   QVector<STATE_DATA> m_vStateData; // 状态数据

   int m_nFP;  //数据帧中FP的数量
   int m_nLight; //数据帧中Light的数量
   int m_nSnore; //数据帧中Snore的数量
   int m_nGroAcc;//数据帧中GorACC的数量


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

   void insertData(QVector<SD_FP> &list,SD_FP data);
   void insertData(QVector<SD_LIGTH> &list,SD_LIGTH data);
   void insertData(QVector<double> &list,double data);
   void insertData(QVector<SD_SNORE> &list,SD_SNORE data);
   void insertData(QVector<SD_GRO> &list,SD_GRO data);
   void insertData(QVector<SD_ACC> &list,SD_ACC data);
   void insertData(QVector<STATE_DATA> &list,STATE_DATA data);

   void updateFPAmount();
   void updateLightAmount();
   void updateSnoreAmount();
   void updateGROACCAmount();
   void updateAngelAccAmount();

   QByteArray getSendData(char *buffer);

   void calPacketLossRate(int num);
};

#endif // SENSORDATA_H
