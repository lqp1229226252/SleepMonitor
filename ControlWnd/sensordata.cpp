#include "sensordata.h"
#include "malloc.h"
#define MAX_LENGTH 100
#include "math.h"
#include "QDebug"
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
    this->m_nFP1Amount=0;
    this->m_nFP2Amount=0;

    this->m_nRedLightAmount=0;
    this->m_nNearRedLightAmount=0;
    this->m_nGreenLightAmount=0;

    this->m_nLSnoreAmount=0;
    this->m_nRSnoreAmount=0;

    this->m_nSeatAngleAmount=0;
    this->m_nRollAngleAmount=0;
    this->m_nAccAmount=0;

    this->m_nGroxAmount=0;
    this->m_nGroyAmount=0;
    this->m_nGrozAmount=0;
    this->m_nAccxAmount=0;
    this->m_nAccyAmount=0;
    this->m_nAcczAmount=0;


    this->m_nFP=5;
    this->m_nLight=1;
    this->m_nSnore=10;
    this->m_nGroAcc=1;
    this->m_source=0x0000;
    m_vSrcData.resize(0);
}
void SensorData::appendData(QByteArray pData)
{
    this->catData(pData);
    if(findBufHead())
    {
        if(findBufEnd())
        {
//            qDebug()<<"开始解析";
            QByteArray buffer=getBuffer();
            paraData(buffer);
            quint8 buf_len=quint8(buffer[1]+(buffer[2]<<8));
            calPacketLossRate(buffer[buf_len-2]);
        }
    }
}

void SensorData::setCtrlData(int nFP,int n_Light,int n_Snore,int n_GroAcc,quint16 m_source)
{
    this->m_nFP=nFP;
    this->m_nSnore=n_Snore;
    this->m_nLight=n_Light;
    this->m_nGroAcc=n_GroAcc;
    this->m_source=m_source;
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

QVector<SD_FP> SensorData::getFP1()
{
    return this->m_vFP1;
}

QVector<SD_FP> SensorData::getFP2()
{
    return this->m_vFP2;
}

QVector<SD_LIGTH> SensorData::getRedLight()
{
    return this->m_vRedLight;
}

QVector<SD_LIGTH> SensorData::getNearReadLight()
{
    return this->m_vNearRedLight;
}

QVector<SD_LIGTH> SensorData::getGreenLight()
{
    return this->m_vGreenLight;
}

QVector<SD_SNORE> SensorData::getSnoreLeft()
{
    return this->m_vLSnore;
}

QVector<SD_SNORE> SensorData::getSnoreRight()
{
    return this->m_vRSnore;
}

QVector<double> SensorData::getRollAngel()
{
    return this->m_vRollAngle;
}

QVector<double> SensorData::getSeatAngle()
{
    return this->m_vSeatAngle;
}

QVector<double> SensorData::getActionAcc()
{
    return this->m_vAcceleration;
}

QVector<SD_GRO> SensorData::getGrox()
{
    return this->m_vXgro;
}

QVector<SD_GRO> SensorData::getGroy()
{
    return this->m_vYgro;
}

QVector<SD_GRO> SensorData::getGroz()
{
    return this->m_vZgro;
}

QVector<SD_ACC> SensorData::getAccx()
{
    return this->m_vXacc;
}

QVector<SD_ACC> SensorData::getAccy()
{
    return this->m_vYacc;
}

QVector<SD_ACC> SensorData::getAccz()
{
    return this->m_vZacc;
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
    quint8 buf_len=quint8(this->m_vSrcData[1]+(this->m_vSrcData[2]<<8));
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
    quint8 buf_len=quint8(this->m_vSrcData.at(1)+(this->m_vSrcData.at(2)<<8));
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
        emit(stataSignal(stata_data));
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
    QVector<double> vector(m_nFP*2+m_nLight*3+m_nSnore*2+3+m_nGroAcc*6);
    int data_index=0;
    //插入FP数据
    for(int i=0;i<m_nFP;i++)
    {
        //插入FP1数据
         if(this->m_source&0x8000)
         {
             int FP1_index=index;
             data=getdata(buffer,FP1_index,index+4);
             SD_FP fp1=paraFPData(data);
             vector[data_index]=fp1.dbVal;
             insertData(this->m_vFP1,fp1,m_nFP1Amount);
             index+=4;
             updateFP1Amount();
         }
         else
         {
             vector[data_index]=0;
         }
         data_index++;

    }

    for(int i=0;i<m_nFP;i++)
    {
        //插入FP2
        if(this->m_source&0x4000)
        {
            int FP2_index=index;
            data=getdata(buffer,FP2_index,FP2_index+4);
            SD_FP fp2=paraFPData(data);
            vector[data_index]=fp2.dbVal;
            insertData(this->m_vFP2,fp2,m_nFP2Amount);
            index+=4;
            updateFP2Amount();
        }
        else
        {
             vector[data_index]=0;
        }

        data_index++;
    }

    //插入灯光数据

    for(int i=0;i<m_nLight;i++)
    {
        //解析红光数据
        if(this->m_source&0x2000)
        {
            int red_index=index;
            data=getdata(buffer,red_index,red_index+3);
            SD_LIGTH light_red=paraLightData(data);
            vector[data_index]=light_red.dbVal;
            insertData(this->m_vRedLight,light_red,m_nRedLightAmount);
            index+=3;
            updateRedLightAmount();
        }
        else
        {
            vector[data_index]=0;
        }

         data_index++;
    }

    for(int i=0;i<m_nLight;i++)
    {
         //解析近红光数据
        if(this->m_source&0x1000)
        {
            int near_index=index;
            data=getdata(buffer,near_index,near_index+3);
            SD_LIGTH light_near_red=paraLightData(data);
            vector[data_index]=light_near_red.dbVal;
            insertData(this->m_vNearRedLight,light_near_red,m_nNearRedLightAmount);
            index+=3;
            updateNearRedLightAmount();
        }
        else
        {
            vector[data_index]=0;
        }

         data_index++;
    }
    for(int i=0;i<m_nLight;i++)
    {
        //解析绿光数据
        if(this->m_source&0x0800)
        {
            int green_index=index;
            data=getdata(buffer,green_index,green_index+3);
            SD_LIGTH light_green=paraLightData(data);
            vector[data_index]=light_green.dbVal;
            insertData(this->m_vGreenLight,light_green,m_nGreenLightAmount);
            index+=3;
            updateGreenLightAmount();
        }
        else
        {
            vector[data_index]=0;
        }

         data_index++;
    }


    //解析坐立角数据
    if(this->m_source&0x0400)
    {
        data=getdata(buffer,index,index+2);
        double seat_angle=paraAngleData(data);
        vector[data_index]=seat_angle;
        insertData(this->m_vSeatAngle,seat_angle,m_nSeatAngleAmount);
        index+=2;
        updateSeatAngelAmount();
    }
    else
    {
       vector[data_index]=0;
    }
    data_index++;
    //解析翻滚角数据
    if(this->m_source&0x0200)
    {
        data=getdata(buffer,index,index+2);
        double roll_angel=paraAngleData(data);
        vector[data_index]=roll_angel;
        insertData(this->m_vRollAngle,roll_angel,m_nRollAngleAmount);
        index+=2;
        updateRollAngelAmount();
    }
    else
    {
        vector[data_index]=0;
    }
    data_index++;
    //解析运动加速度数据
    if(this->m_source&0x0100)
    {
        data=getdata(buffer,index,index+2);
        double action_acc=paraActionAccData(data);
        vector[data_index]=action_acc;
        insertData(this->m_vAcceleration,action_acc,m_nAccAmount);
        index+=2;
        updateAccAmount();
    }
    else
    {
        vector[data_index]=0;
    }
    data_index++;


    for(int i=0;i<m_nSnore;i++)
    {
        //解析左鼾声数据
        if(this->m_source&0x0080)
        {
            data=getdata(buffer,index,index+3);
            SD_SNORE snore_left=paraSnoreData(data);
            vector[data_index]=snore_left.dbVal;
            insertData(this->m_vLSnore,snore_left,m_nLSnoreAmount);
            index+=3;
            updateLSnoreAmount();
        }
        else
        {
             vector[data_index]=0;
        }
        data_index++;
    }
    for(int i=0;i<m_nSnore;i++)
    {
         //解析右鼾声数据
        if(this->m_source&0x0040)
        {
            int right_index=index;
            data=getdata(buffer,right_index,right_index+3);
            SD_SNORE snore_right=paraSnoreData(data);
            vector[data_index]=snore_right.dbVal;
            insertData(this->m_vRSnore,snore_right,m_nRSnoreAmount);
            index+=3;
            updateRSnoreAmount();
        }
        else
        {
             vector[data_index]=0;
        }
        data_index++;

    }

    for(int i=0;i<m_nGroAcc;i++)
    {
        //解析x-gro数据
        if(this->m_source&0x0020)
        {
            data=getdata(buffer,index,index+3);
            SD_GRO gro_x=paraGroData(data);
            insertData(this->m_vXgro,gro_x,m_nGroxAmount);
            vector[data_index]=gro_x.dbVal;
            index+=3;
            updateGROXAmount();
        }
        else
        {
            vector[data_index]=0;
        }
        data_index++;
    }
    for(int i=0;i<m_nGroAcc;i++)
    {
        //解析y-gro数据
        if(this->m_source&0x0010)
        {
            data=getdata(buffer,index,index+3);
            SD_GRO gro_y=paraGroData(data);
            insertData(m_vYgro,gro_y,m_nGroyAmount);
            vector[data_index]=gro_y.dbVal;
            index+=3;
            updateGROYAmount();
        }
        else
        {
            vector[data_index]=0;
        }
        data_index++;
    }
    for(int i=0;i<m_nGroAcc;i++)
    {
        //解析z-gro数据
        if(this->m_source&0x0008)
        {
            data=getdata(buffer,index,index+3);
            SD_GRO gro_z=paraGroData(data);
            insertData(m_vZgro,gro_z,m_nGrozAmount);
            vector[data_index]=gro_z.dbVal;
            index+=3;
            updateGROZAmount();
        }
        else
        {
            vector[data_index]=0;
        }
        data_index++;
    }
    for(int i=0;i<m_nGroAcc;i++)
    {
       //解析x-acc数据
        if(this->m_source&0x0004)
        {
            data=getdata(buffer,index,index+3);
            SD_ACC acc_x=paraAccData(data);
            insertData(m_vXacc,acc_x,m_nAccxAmount);
            vector[data_index]=acc_x.dbVal;
            index+=3;
            updateACCXAmount();
        }
        else
        {
            vector[data_index]=0;
        }
        data_index++;
    }
    for(int i=0;i<m_nGroAcc;i++)
    {
       //解析y-acc数据
        if(this->m_source&0x0002)
        {
            data=getdata(buffer,index,index+3);
            SD_ACC acc_y=paraAccData(data);
            insertData(m_vYacc,acc_y,m_nAccyAmount);
            vector[data_index]=acc_y.dbVal;
            index+=3;
            updateACCYAmount();
        }
        else
        {
            vector[data_index]=0;
        }
        data_index++;
    }
    for(int i=0;i<m_nGroAcc;i++)
    {
       //解析y-acc数据
        if(this->m_source&0x0001)
        {
            data=getdata(buffer,index,index+3);
            SD_ACC acc_z=paraAccData(data);
            insertData(m_vZacc,acc_z,m_nAcczAmount);
            vector[data_index]=acc_z.dbVal;
            index+=3;
            updateACCZAmount();
        }
        else
        {
            vector[data_index]=0;
        }
        data_index++;
    }
    emit(dataSignal(vector));

}
SD_FP SensorData::paraFPData(QByteArray buffer)
{
    SD_FP fp;
    Data32 data;
    data.buffer[1]=buffer[1];
    data.buffer[2]=buffer[2];
    data.buffer[3]=buffer[3];
    int value=data.data>>8;
    fp.dbVal=value*0.1;
    fp.nIndex=buffer[0]&0x0E>>1;
    fp.nRemark=buffer[0]&0x01;
    return fp;
}
SD_LIGTH SensorData::paraLightData(QByteArray buffer)
{
    SD_LIGTH light;
    Data32 data;
    data.buffer[1]=buffer[0]&char(0xC0);
    data.buffer[2]=buffer[1];
    data.buffer[3]=buffer[2];
    int value=data.data>>14;
    light.dbVal=value*10;
    light.nIndex=buffer[0]&0x03;
    return light;
}
double SensorData::paraAngleData(QByteArray buffer)
{
    double angel;
    Data16 data;
    data.buffer[0]=buffer[0]&char(0xF0);
    data.buffer[1]=buffer[1];
    int value=data.data>>4;
    angel=value*0.1;
    return angel;
}
double SensorData::paraActionAccData(QByteArray buffer)
{
    double action_acc;
    Data16 data;
    data.buffer[0]=buffer[0]&char(0xF0);
    data.buffer[1]=buffer[1];
    int value=data.data>>4;
    action_acc=value*0.1;
    return action_acc;
}
SD_SNORE SensorData::paraSnoreData(QByteArray buffer)
{
    SD_SNORE snore;
    Data16 data;
    data.buffer[0]=buffer[1];
    data.buffer[1]=buffer[2];
    int value=data.data;
    snore.dbVal=value*0.1;
    snore.nIndex=buffer[0]&0x0F;
    return snore;
}
SD_ACC SensorData::paraAccData(QByteArray buffer)
{
    SD_ACC acc;
    Data16 data;
    data.buffer[0]=buffer[1];
    data.buffer[1]=buffer[2];
    int value=data.data;
    acc.dbVal=value*0.1;
    acc.nIndex=buffer[0]&0x0F;
    return acc;
}
SD_GRO SensorData::paraGroData(QByteArray buffer)
{
    SD_GRO gro;
    Data16 data;
    data.buffer[0]=buffer[2];
    data.buffer[1]=buffer[1];
    int value=data.data;
    gro.dbVal=value*0.1;
    gro.nIndex=buffer[0]&0x0F;
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

void SensorData::insertData(QVector<SD_FP> &list, SD_FP data,int index)
{
    list[index]=data;
}

void SensorData::insertData(QVector<SD_LIGTH> &list, SD_LIGTH data,int index)
{
    list[index]=data;
}

void SensorData::insertData(QVector<double> &list, double data,int index)
{
    list[index]=data;
}

void SensorData::insertData(QVector<SD_SNORE> &list, SD_SNORE data,int index)
{
    list[index]=data;
}

void SensorData::insertData(QVector<SD_GRO> &list, SD_GRO data,int index)
{
    list[index]=data;
}

void SensorData::insertData(QVector<SD_ACC> &list, SD_ACC data,int index)
{
    list[index]=data;

}

void SensorData::insertData(QVector<STATE_DATA> &list, STATE_DATA data)
{
    list.append(data);
}

void SensorData::updateFP1Amount()
{
    m_nFP1Amount++;
    if(m_nFP1Amount==MAX_LENGTH)
    {
        m_nFP1Amount=0;
        emit(FP1OverFlow());
    }
}

void SensorData::updateFP2Amount()
{
    m_nFP2Amount++;
    if(m_nFP2Amount==MAX_LENGTH)
    {
        m_nFP2Amount=0;
        emit(FP2OverFlow());
    }
}

void SensorData::updateRedLightAmount()
{
    m_nRedLightAmount++;
    if(m_nRedLightAmount==MAX_LENGTH)
    {
        m_nRedLightAmount=0;
        emit(redLightOverFlow());
    }
}

void SensorData::updateNearRedLightAmount()
{
    m_nNearRedLightAmount++;
    if(m_nNearRedLightAmount==MAX_LENGTH)
    {
        m_nNearRedLightAmount=0;
        emit(nearRedLightOverFlow());
    }
}

void SensorData::updateGreenLightAmount()
{
    m_nGreenLightAmount++;
    if(m_nGreenLightAmount==MAX_LENGTH)
    {
        m_nGreenLightAmount=0;
        emit(greenLightOverFlow());
    }
}

void SensorData::updateSeatAngelAmount()
{
    m_nSeatAngleAmount++;
    if(m_nSeatAngleAmount==MAX_LENGTH)
    {
        m_nSeatAngleAmount=0;
        emit(seatAngleOverFlow());
    }
}

void SensorData::updateRollAngelAmount()
{
    m_nRollAngleAmount++;
    if(m_nRollAngleAmount==MAX_LENGTH)
    {
        m_nRollAngleAmount=0;
        emit(rollAngleOverFlow());
    }
}

void SensorData::updateAccAmount()
{
    m_nAccAmount++;
    if(m_nAccAmount==MAX_LENGTH)
    {
        m_nAccAmount=0;
        emit(accOverFlow());
    }
}

void SensorData::updateLSnoreAmount()
{
    m_nLSnoreAmount++;
    if(m_nLSnoreAmount==MAX_LENGTH)
    {
        m_nLSnoreAmount=0;
        emit(lSnoreOverFlow());
    }

}

void SensorData::updateRSnoreAmount()
{
    m_nRSnoreAmount++;
    if(m_nRSnoreAmount==MAX_LENGTH)
    {
        m_nRSnoreAmount=0;
        emit(rSnoreOverFlow());
    }
}

void SensorData::updateGROXAmount()
{
    m_nGroxAmount++;
    if(m_nGroxAmount==MAX_LENGTH)
    {
        m_nGroxAmount=0;
        emit(GroXOverFlow());
    }
}

void SensorData::updateGROYAmount()
{
    m_nGroyAmount++;
    if(m_nGroyAmount==MAX_LENGTH)
    {
        m_nGroyAmount=0;
        emit(GroYOverFlow());
    }
}

void SensorData::updateGROZAmount()
{
    m_nGrozAmount++;
    if(m_nGrozAmount==MAX_LENGTH)
    {
        m_nGrozAmount=0;
        emit(GroZOverFlow());
    }
}

void SensorData::updateACCXAmount()
{
    m_nAccxAmount++;
    if(m_nAccxAmount==MAX_LENGTH)
    {
        m_nAccxAmount=0;
        emit(ACCXOverFlow());
    }
}

void SensorData::updateACCYAmount()
{
    m_nAccyAmount++;
    if(m_nAccyAmount==MAX_LENGTH)
    {
        m_nAccyAmount=0;
        emit(ACCYOverFlow());
    }
}

void SensorData::updateACCZAmount()
{
    m_nAcczAmount++;
    if(m_nAcczAmount==MAX_LENGTH)
    {
        m_nAcczAmount=0;
        emit(ACCZOverFlow());
    }
}

QByteArray SensorData::getSendData(char *buffer)
{
     quint8 buf_len=quint8(buffer[1]+(buffer[2]<<8));
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
