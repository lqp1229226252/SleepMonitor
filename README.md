# SleepMonitor

## 修改 7.26

修改eegwnd的窗口大小SleepMonitor::SleepMonitor<br>
修改对于接收数据的处理ControlWnd::dataPlot(QVector<double> data)

## 修改 7.27

修改曲线窗口最大化问题<br>
增加文件转存功能<br>
注：Qt5.14.2QbyetArray访问时溢出不会终止程序，但会打印信息，可能会导致系统卡死

## 修改 7.30

### 修改解析

对于数据解析添加是否该数据源是否被采集。

同时修改sensordata类成员，将数据的数据量记录修改为每个数据都有单独的数据量记录。

文件保存修改为只保存一列。

修改了曲线的显示问题

## 修改 8.1

修改曲线卡死问题和曲线最大化后位置不对的问题