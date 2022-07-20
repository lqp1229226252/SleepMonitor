#include "sensorData.h"
#include "malloc.h"
#define MAX_LENGTH 100
#include "math.h"
#include "QDebug"

#if _MSC_VER >= 1600

#pragma execution_character_set("utf-8")

#endif

SensorData::SensorData(QObject *object):QObject(object)
{
    m_vFP1.resize(MAX_LENGTH);
    m_vFP2.resize(MAX_LENGTH);

    m_vRedLight.resize(MAX_LENGTH);
    m_vNearRedLight.resize(MAX_LENGTH);
    m_vGreenLight.resize(MAX_LENGTH);

    m_vSeatAngle.resize(MAX_LENGTH);
    m_vRollAngle.resize(MAX_LENGTH);
    m_vAcceleration.resize(MAX_LENGTH);

    m_vLSnore.resize(MAX_LENGTH);
    m_vRSnore.resize(MAX_LENGTH);

    m_vXgro.resize(MAX_LENGTH);
    m_vYgro.resize(MAX_LENGTH);
    m_vZgro.resize(MAX_LENGTH);

    m_vXacc.resize(MAX_LENGTH);
    m_vYacc.resize(MAX_LENGTH);
    m_vZacc.resize(MAX_LENGTH);
    this->m_nFPAmount=0;
    this->m_nSnoreAmount=0;
    this->m_nLightAmount=0;
    this->m_nGroAccAmount=0;
    this->m_nAngleAccAmount=0;

    this->m_nFP=5;
    this->m_nLight=1;
    this->m_nSnore=10;
    this->m_nGroAcc=1;
}
void SensorData::appendData(QByteArray pData)
{
    this->catData(pData);
    if(findBufHead())
    {
        if(findBufEnd())
        {
            qDebug()<<"开始解析";
            QByteArray buffer=getBuffer();
            paraData(buffer);
            int buf_len=buffer[1]+(buffer[2]<<8);
            calPacketLossRate(buffer[buf_len-2]);
        }
    }
}

void SensorData::setCtrlData(int nFP,int n_Snore,int n_Light,int n_GroAcc)
{
    this->m_nFP=nFP;
    this->m_nSnore=n_Snore;
    this->m_nLight=n_Light;
    this->m_nGroAcc=n_GroAcc;
}

int SensorData::getFPNum()
{
    return this->m_nFP;
}

int SensorData::getLightNum()
{
    return this->m_nLight;
}

int SensorData::getSnoreNum()
{
    return this->m_nSnore;
}

int SensorData::getGroAccNum()
{
    return this->m_nGroAcc;
}
void SensorData::catData(QByteArray pData)
{
    for(int i=0;i<pData.size();i++)
    {
        this->m_vSrcData.append(pData[i]);
    }
}
bool SensorData::findBufHead()
{
    int i=0;
    while(this->m_vSrcData.at(i)!=char(0xBE))
    {
        i++;
    }
    if(i<this->m_vSrcData.size())
    {
        this->m_vSrcData.erase(m_vSrcData.begin(),m_vSrcData.begin()+i);
        return true;
    }
    else
    {
        return false;
    }
}
bool SensorData::findBufEnd()
{
    int buf_len=this->m_vSrcData[1]+(this->m_vSrcData[2]<<8);
    if(this->m_vSrcData.size()<buf_len)
    {
        return false;
    }
    quint8 buf_end=quint8(this->m_vSrcData[buf_len-1]);
    if(buf_end!=0xEB)
    {
        this->m_vSrcData.erase(m_vSrcData.begin());
        return false;
    }
    return true;

}

QByteArray SensorData::getBuffer()
{
    int buf_len=this->m_vSrcData.at(1)+(this->m_vSrcData.at(2)<<8);
    QByteArray buffer;
    for(int i=0;i<buf_len;i++)
    {
       buffer.append(this->m_vSrcData[i]);
    }
    this->m_vSrcData.erase(m_vSrcData.begin(),m_vSrcData.begin()+buf_len);
    return buffer;
}
void SensorData::paraData(QByteArray buffer)
{
    quint8 code=quint8(buffer[3]);
    if(code==0x00)
    {
        QByteArray data=getdata(buffer,4,buffer.size()-3);
        paraAllData(data);
        emit(dataSignal(data));
    }
    if(code==0x01)
    {
        QByteArray data=getdata(buffer,4,buffer.size()-3);
        STATE_DATA stata_data=paraStateData(data);
        //插入
        insertData(this->m_vStateData,stata_data);
        emit(stataSignal(data));
    }
}
QByteArray SensorData::getdata(QByteArray buffer, int start, int end)
{
    QByteArray data;
    for(int i=start;i<end;i++)
    {
        data.append(buffer[i]);
    }
    return data;
}
void SensorData::paraAllData(QByteArray buffer)
{
    int index=0;
    QByteArray data;
    //插入FP1数据
    for(int i=0;i<m_nFP;i++)
    {
         data=getdata(buffer,index,index+4);
         SD_FP fp1=paraFPData(data);
         insertData(this->m_vFP1,fp1);
         index+=4;
    }
    //插入FP2数据
    for(int i=0;i<m_nFP;i++)
    {
         data=getdata(buffer,index,index+4);
         SD_FP fp2=paraFPData(data);
         insertData(this->m_vFP2,fp2);
         index+=4;
         updateFPAmount();
    }
    //解析红光数据
    for(int i=0;i<m_nLight;i++)
    {
         data=getdata(buffer,index,index+3);
         SD_LIGTH light_red=paraLightData(data);
         insertData(this->m_vRedLight,light_red);
         index+=3;
    }
    //解析近红光数据
    for(int i=0;i<m_nLight;i++)
    {
         data=getdata(buffer,index,index+3);
         SD_LIGTH light_near_red=paraLightData(data);
         insertData(this->m_vNearRedLight,light_near_red);
         index+=3;
    }
    //解析绿光数据
    for(int i=0;i<m_nLight;i++)
    {
         data=getdata(buffer,index,index+3);
         SD_LIGTH light_green=paraLightData(data);
         insertData(this->m_vGreenLight,light_green);
         index+=3;
         updateLightAmount();
    }

    //解析坐立角数据
    data=getdata(buffer,index,index+2);
    double seat_angle=paraAngleData(data);
    insertData(this->m_vSeatAngle,seat_angle);
    index+=2;
    //解析翻滚角数据
   data=getdata(buffer,index,index+2);
    double roll_angel=paraAngleData(data);
    insertData(this->m_vRollAngle,roll_angel);
    index+=2;
    //解析运动加速度数据
    data=getdata(buffer,index,index+2);
    double action_acc=paraActionAccData(data);
    insertData(this->m_vAcceleration,action_acc);
    index+=2;
    updateAngelAccAmount();

    for(int i=0;i<m_nSnore;i++)
    {
        //解析左鼾声数据
        data=getdata(buffer,index,index+3);
        SD_SNORE snore_left=paraSnoreData(data);
        insertData(this->m_vLSnore,snore_left);
        //解析右鼾声数据
        int right_index=index+m_nSnore*3;
        data=getdata(buffer,right_index,right_index+3);
        SD_SNORE snore_right=paraSnoreData(data);
        insertData(this->m_vRSnore,snore_right);
        index+=3;
        updateSnoreAmount();
    }
    index+=m_nSnore*3;
    for(int i=0;i<m_nGroAcc;i++)
    {
        //解析x-gro数据
        data=getdata(buffer,index,index+3);
        SD_GRO gro_x=paraGroData(data);
        insertData(this->m_vXgro,gro_x);
         //解析y-gro数据
        int temp_index=index+m_nGroAcc*3;
        data=getdata(buffer,temp_index,temp_index+3);
        SD_GRO gro_y=paraGroData(data);
        insertData(m_vYgro,gro_y);
         //解析z-gro数据
        temp_index=index+m_nGroAcc*3*2;
        data=getdata(buffer,temp_index,temp_index+3);
        SD_GRO gro_z=paraGroData(data);
         insertData(m_vZgro,gro_z);
        //解析x-acc数据
         temp_index=index+m_nGroAcc*3*3;
         data=getdata(buffer,temp_index,temp_index+3);
        SD_ACC acc_x=paraAccData(data);
        insertData(m_vXacc,acc_x);
         //解析y-acc数据
        temp_index=index+m_nGroAcc*3*4;
        data=getdata(buffer,temp_index,temp_index+3);
        SD_ACC acc_y=paraAccData(data);
        insertData(m_vYacc,acc_y);
         //解析z-acc数据
        temp_index=index+m_nGroAcc*3*5;
        data=getdata(buffer,temp_index,temp_index+3);
        SD_ACC acc_z=paraAccData(data);
        insertData(m_vZacc,acc_z);
        updateGROACCAmount();
    }


}
SD_FP SensorData::paraFPData(QByteArray buffer)
{
    SD_FP fp;
    int value=buffer[0]+(buffer[1]<<8)+(buffer[2]<<16);
    fp.dbVal=value*0.1;
    fp.nIndex=buffer[4]&0x0E;
    fp.nRemark=buffer[4]&0x01;
    return fp;
}
SD_LIGTH SensorData::paraLightData(QByteArray buffer)
{
    SD_LIGTH light;
    int value=buffer[0]+(buffer[1]<<8)+((buffer[2]&0xC0)<<10);
    light.dbVal=value*10;
    light.nIndex=buffer[3]&0x03;
    return light;
}
double SensorData::paraAngleData(QByteArray buffer)
{
    double angel;
    int value=buffer[0]+((buffer[1]&0xF0)<<4);
    angel=value*0.1;
    return angel;
}
double SensorData::paraActionAccData(QByteArray buffer)
{
    double action_acc;
    int value=buffer[0]+((buffer[1]&0xF0)<<4);
    action_acc=value*0.1;
    return action_acc;
}
SD_SNORE SensorData::paraSnoreData(QByteArray buffer)
{
    SD_SNORE snore;
    int value=buffer[0]+(buffer[1]<<8);
    snore.dbVal=value*0.1;
    snore.nIndex=buffer[3]&0x0F;
    return snore;
}
SD_ACC SensorData::paraAccData(QByteArray buffer)
{
    SD_ACC acc;
    int value=buffer[0]+(buffer[1]<<8);
    acc.dbVal=value*0.1;
    acc.nIndex=buffer[3]&0x0F;
    return acc;
}
SD_GRO SensorData::paraGroData(QByteArray buffer)
{
    SD_GRO gro;
    int value=buffer[0]+(buffer[1]<<8);
    gro.dbVal=value*0.1;
    gro.nIndex=buffer[3]&0x0F;
    return gro;
}
STATE_DATA SensorData::paraStateData(QByteArray buffer)
{
    STATE_DATA state;
    state.nVol=quint8(buffer[0])*20;
    state.bRecharge=buffer[1];
    state.dbTemp=buffer[2]*0.2;
    state.nTime=quint8(buffer[3])+(quint8(buffer[4])<<8)+(quint8(buffer[5])<<16)+(quint8(buffer[6])<<24);
    state.nCapacity=quint8(buffer[7])*2;
    state.nResidual=quint8(buffer[8])*2;
    return state;
}

void SensorData::insertData(QVector<SD_FP> &list, SD_FP data)
{
    list[m_nFPAmount]=data;
}

void SensorData::insertData(QVector<SD_LIGTH> &list, SD_LIGTH data)
{
    list[m_nLightAmount]=data;
}

void SensorData::insertData(QVector<double> &list, double data)
{
    list[m_nAngleAccAmount]=data;
}

void SensorData::insertData(QVector<SD_SNORE> &list, SD_SNORE data)
{
    list[m_nSnoreAmount]=data;
}

void SensorData::insertData(QVector<SD_GRO> &list, SD_GRO data)
{
    list[m_nGroAccAmount]=data;
}

void SensorData::insertData(QVector<SD_ACC> &list, SD_ACC data)
{
    list[m_nGroAccAmount]=data;

}

void SensorData::insertData(QVector<STATE_DATA> &list, STATE_DATA data)
{
    list.append(data);
}

void SensorData::updateFPAmount()
{
    m_nFPAmount++;
    if(m_nFPAmount==MAX_LENGTH)
    {
        m_nFPAmount=0;
    }
}

void SensorData::updateLightAmount()
{
    m_nLightAmount++;
    if(m_nLightAmount==MAX_LENGTH)
    {
        m_nLightAmount=0;
    }
}

void SensorData::updateSnoreAmount()
{
    m_nSnoreAmount++;
    if(m_nSnoreAmount==MAX_LENGTH)
    {
        m_nSnoreAmount=0;
    }

}

void SensorData::updateGROACCAmount()
{
    m_nGroAccAmount++;
    if(m_nGroAccAmount==MAX_LENGTH)
    {
        m_nGroAccAmount=0;
    }

}

void SensorData::updateAngelAccAmount()
{
    m_nAngleAccAmount++;
    if(m_nAngleAccAmount==MAX_LENGTH)
    {
        m_nAngleAccAmount=0;
    }
}

QByteArray SensorData::getSendData(char *buffer)
{
     int buf_len=buffer[1]+(buffer[2]<<8);
     QByteArray data;
     for(int i=4;i<(buf_len-3);i++)
     {
         data.append(buffer[i]);
     }
     return data;
}

void SensorData::calPacketLossRate(int num)
{
    float this_loss_rate=0;
    static float last_loss_rate=0;
    static int last_num=0;
    int this_num=num;
    static int success_buf=0;
    float loss=0;
    if(this_num-last_num==1)
    {
        success_buf++;
        last_loss_rate=this_loss_rate;
    }
    else {
       loss=std::fabs(last_num-this_num-1);
    }
    this_loss_rate=(loss/(success_buf+loss))*100.0f;
    if(last_loss_rate!=this_loss_rate)
    {
        emit(lossRateChange(this_loss_rate));
    }

}
