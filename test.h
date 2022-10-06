#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();

protected:
    void mousePressEvent(QMouseEvent *event);		//鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event);		//鼠标移动事件
    void wheelEvent(QWheelEvent *event);			//滚轮事件


private:
    Ui::test *ui;
};

#endif // TEST_H
