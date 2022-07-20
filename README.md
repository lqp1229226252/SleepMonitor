SensorData现定义在commsetwidget为weisensordata<br>
sensordata的信号已绑定在commsetwidget中并且会发出信号，信号如下:
```
    void dataSignal(QByteArray); //睡眠贴数据信息
    void stataSignal(QByteArray); //状态数据
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
int的参数表示那个文件
0：FP
1:light
2:angel_acc
3:snore
4:gro_acc
```
## 文件转存
```
fileStorageState(bool) //true表示发送开始转存，false表示结束转存储
```
