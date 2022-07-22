#ifndef CONTROLDATA_H
#define CONTROLDATA_H
// #include "QObject"
#include "struct_data.h"
#include "QVector"
class ControlData
{
private:
    /* data */
    CTRL_DATA m_CtrlData;
    SD_AMOUNT m_SensorDataAmount;
    QString m_strPathFP;
    QString m_strPathLight;
    QString m_strPathAngleAcc;
    QString m_strPathSnore;
    QString m_strPathGroAcc;
    bool storage_falg;
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
    bool getSaveFlag();

    void SaveBegin(QStringList paths);
    void SaveEnd();
    void saveFP(QVector<SD_FP>,char flag);
    void saveLight(QVector<SD_LIGTH>,char flag);
    void saveAngleAcc(QVector<double>,char flag);
    void saveSnore(QVector<SD_SNORE>,char flag);
    void saveGro(QVector<SD_GRO>,char flag);
    void saveACC(QVector<SD_ACC>,char flag);

};
#endif
