#ifndef CONTROLDATA_H
#define CONTROLDATA_H
// #include "QObject"
#include "struct_data.h"
#include "QVector"
#include "QObject"
typedef struct stride
{
    int fp;
    int light;
    int snore;
    int groacc;
}Stride;
class ControlData:public QObject
{
    Q_OBJECT
private:
    /* data */
    CTRL_DATA m_CtrlData;
    SD_AMOUNT m_SensorDataAmount;

    QString m_strPathFP1;
    QString m_strPathFP2;

    QString m_strPathRedLight;
    QString m_strPathRedNearLight;
    QString m_strPathGreenLight;

    QString m_strPathSeatAngle;
    QString m_strPathRollAngle;
    QString m_strPathACC;

    QString m_strPathLSnore;
    QString m_strPathRSnore;

    QString m_strPathGroX;
    QString m_strPathGroY;
    QString m_strPathGroZ;

    QString m_strPathAccX;
    QString m_strPathAccY;
    QString m_strPathAccZ;

    bool storage_falg;
    Stride stride;
    int data_Maxnum;//数据最大数目
public:
    explicit ControlData(QObject *parent=nullptr);
//    ~ControlData();
    void setCtrlData(quint8,quint8,quint16,quint16);
    void setSensorDataAmount(int,int,int,int,qint16);
    void setStride();
    CTRL_DATA getCtrlData();
    CTRL_DATA* getCtrlDate();
    SD_AMOUNT getSensorDataAmount();
    Stride getStride();
    int getMaxNum();
    int getFPAmount();
    int getSnoreAmount();
    int getLightAmount();
    int getGroAccAmount();
    qint16 getFrameLen();



    bool getSaveFlag();
    void checkFileExit(QString path);

    void saveFP(QString path,QVector<SD_FP>);
    void saveLight(QString path,QVector<SD_LIGTH>);
    void saveAngleAcc(QString path,QVector<double>);
    void saveSnore(QString path,QVector<SD_SNORE>);
    void saveGro(QString path,QVector<SD_GRO>);
    void saveACC(QString path, QVector<SD_ACC>);

    //存储fp数据
    void saveFP1(QVector<SD_FP>);
    void saveFP2(QVector<SD_FP>);
    //存储灯光数据
    void saveRedLight(QVector<SD_LIGTH>);
    void saveRedNearLight(QVector<SD_LIGTH>);
    void saveGreenLight(QVector<SD_LIGTH>);
    //存储鼾声数据
    void saveLSnore(QVector<SD_SNORE>);
    void saveRSnore(QVector<SD_SNORE>);
    //存储角度和加速度
    void saveSeatAngle(QVector<double>);
    void saveRollAngel(QVector<double>);
    void saveAcc(QVector<double>);
    //存储Gro
    void saveGrox(QVector<SD_GRO>);
    void saveGroy(QVector<SD_GRO>);
    void saveGroz(QVector<SD_GRO>);
    //存储acc
    void saveACCx(QVector<SD_ACC>);
    void saveACCy(QVector<SD_ACC>);
    void saveAccz(QVector<SD_ACC>);
signals:
public slots:
    void SaveBegin(QString prefix);
    void SaveEnd();
};
#endif
