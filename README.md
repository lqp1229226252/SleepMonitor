SensorData现定义在commsetwidget为weisensordata<br>
sensordata的信号已绑定在commsetwidget中并且会发出信号，信号如下:
```
    void dataSignal(QByteArray); //睡眠贴数据信息
    void dataSignal(QVector<double>)
    void stataSignal(QByteArray); //状态数据
    void stataSignal(STATE_DATA);
    void lossRateChange(float);  //丢包率
```
如果帧中的数目发生改变可通过setCtrlData(int,int,int,int)，参数为FP、鼾声、灯光、gro-acc的数目<br>
串口发送数据通过commsetWiget的write(QByteArray)发送

# 新增
新增三个按钮：文件路径设置、文件转存开始和文件转存结束三个按钮<br>
## 文件路径设置
点击设置后会弹出一个模态框用于设置文件路径<br>
```
void updatafilePath();//当模态框关闭时触发并且发送FilePathChange(QStringList);包括全部的文件路径
void updatefilePath(int,QString);//当路径被修改时触发，并且发送void FilePathChange(int,QString);
int的参数表示哪个文件
0：FP
1:light
2:angel_acc
3:snore
4:gro_acc
QStringlist getAllPath()//获取文件的全部路径 
```

## 文件转存
```
fileStorageState(bool) //true表示发送开始转存，false表示结束转存储
```
# 文件转存示例
## 信号连接
```
    connect(comm_widget,SIGNAL(fileStart(QString)),ctrl_data->getCtrlData(),SLOT(SaveBegin(QString)));//文件转存信号与ctrldata的savebegin连接
    connect(comm_widget,SIGNAL(fileEnd()),ctrl_data->getCtrlData(),SLOT(SaveEnd()));//文件转存结束信号
    connect(comm_widget->getSensorData(),SIGNAL(FPOverFlow()),this,SLOT(FPOverFlowSlot()));//接收数据溢出信号后开始存储数据到文件
```
## 存储
例如FP数据
```
 ControlData *ctrl=ctrl_data->getCtrlData(); //获取ctrl的地址
    if(ctrl->getSaveFlag())//查看转存状态
    {
        //获取数据
        SensorData *data=comm_widget->getSensorData();
        QVector<SD_FP> FP1=data->getFP1();
        QVector<SD_FP> FP2=data->getFP2();
        //将数据存储在csv
        ctrl->saveFP1(FP1);
        ctrl->saveFP2(FP2);
        qDebug()<<"转存完毕";
    }
    else {
        qDebug()<<"转存没开始";
    }
```