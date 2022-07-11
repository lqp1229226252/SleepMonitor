# ContralDataWidget
通过setSize()设置窗口大小，适合大小700*200，大小可以微调<br>
通过setFontSize设置字体大小，默认是8<br>
通过sendSignal信号获取控制帧，类型为QByteArray，可直接通过串口发送<br>
现在将ControlData集成在里面定义为ctrldata<br>
触发发送按钮后，ctrldata里面的属性值会自动设置<br>
```
 SD_AMOUNT getSensorDataAmount(); //获取帧中的各类数据的数目
    CTRL_DATA getCtrlData();//获取控制帧的信息
```
