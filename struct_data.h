#ifndef STRUCT_DATA_H
#define STRUCT_DATA_H
#include "QObject"
typedef struct _CTRL_DATA { 
   quint8 nCtrl; // 采集控制 
   quint8 nRate; // 采样率 
   quint16 nSrc; // 采样数据源 
   quint16 nAmont; // 采样数据点数 
} CTRL_DATA;
typedef struct _SD_AMOUNT { 
   int nFP; // FP1/FP2 
   int nSnore; // 鼾声 
   int nLight; // 红光/近红光/绿光 
   int nGroAcc; // 各种 gro/acc 
   qint16 nFrameLen; // 总帧数据长度 
}SD_AMOUNT;
typedef struct _SD_FP { 
   double dbVal; // 解析出来的数据，由于单位是 0.1uV，所以需要乘以 0.1 得到这个 
   int nIndex; // 采样序号 
   int nRemark; // 备注，这个本来可以用 bool，但是不知道是否有其它用处 
}SD_FP;
typedef struct _SD_LIGHT { 
   double dbVal; // 原始单位 10uv，所以需要乘以 10 得到这个结果 
   int nIndex; // 采样序号 
}SD_LIGTH;
typedef struct _SD_SNORE { 
   double dbVal; // 原始单位 0.1mV，乘以 0.1 得到该结果 
   int nIndex; // 采样序号 
}SD_SNORE;
typedef struct _SD_GRO { 
   double dbVal; // 原始单位 4mdps，乘以 4 得到该结果 
   int nIndex; // 采样序号 
}SD_GRO;
typedef struct _SD_ACC { 
   double dbVal; // 原始单位 0.06mg，乘以 0.06 得到该结果 
   int nIndex; // 采样序号 
}SD_ACC;
typedef struct _STATE_DATA { 
   unsigned int nVol; // 电池电压，原始单位 20mV，乘以 20 得到该数据 
   bool bRecharge; // 充电状态，原始数据为 0、1 
   double dbTemp; // 设备温度，原始单位 0.2，乘以 0.2 得到 
   unsigned int nTime; // 设定当前时间 
   unsigned int nCapacity; // 容量，原始单位 2GB，乘以 2 得到 
   unsigned int nResidual; // 剩余容量，原始单位 2GB，乘以 2 得到 
}STATE_DATA;
#endif