# SleepMonitor
## 修改 7.26
修改eegwnd的窗口大小SleepMonitor::SleepMonitor<br>
修改对于接收数据的处理ControlWnd::dataPlot(QVector<double> data)
## 修改 7.27
修改曲线窗口最大化问题<br>
增加文件转存功能<br>
注：Qt5.14.2QbyetArray访问时溢出不会终止程序，但会打印信息，可能会导致系统卡死