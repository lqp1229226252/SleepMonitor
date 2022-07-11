SensorData现定义在commsetwidget为weisensordata<br>
sensordata的信号已绑定在commsetwidget中并且会发出信号，信号如下:
```
    void dataSignal(QByteArray); //睡眠贴数据信息
    void stataSignal(QByteArray); //状态数据
    void lossRateChange(float);  //丢包率
```
如果帧中的数目发生改变可通过setCtrlData(int,int,int,int)，参数为FP、鼾声、灯光、gro-acc的数目<br>
串口发送数据通过commsetWiget的write(QByteArray)发送
