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
# 修改ControlData类
# 新增类成员
```
    QString m_strPathFP; //FP的存储路径
    QString m_strPathLight;//light的存储路径
    QString m_strPathAngleAcc;//angleacc的存储路径
    QString m_strPathSnore;//snore的存储路径
    QString m_strPathGroAcc;//groacc的存储路径
    bool storage_falg;//是否转存的标准位
```
# 新增成员函数
```
    void SaveBegin(QStringList paths);//CommSetWidget新增了FilePathChange()能够获取文件的存储路径数组
    void SaveEnd();//将转存的标志置为false
    void saveFP(QVector<SD_FP>);//以插入方式插入FP数据，
    void saveLight(QVector<SD_LIGTH>);//以插入方式插入light数据
    void saveAngleAcc(QVector<double>);以插入方式插入angleacc数据
    void saveSnore(QVector<SD_SNORE>);以插入方式插入snore数据
    void saveGro(QVector<SD_GRO>);以插入方式插入gro数据
    void saveACC(QVector<SD_ACC>);以插入方式插入acc数据
```
