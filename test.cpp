#include "test.h"
#include "ui_test.h"
#include <QDebug>

test::test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    QCursor cursor;                     //创建光标对象
    cursor.setShape(Qt::OpenHandCursor);//设置光标形状(小手)
    setCursor(cursor);
}

test::~test()
{
    delete ui;
}

void test::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){    //左键按下
        qDebug()<<"左键";
    }
    else if(event->button()==Qt::RightButton)//如果是右键按下
    {
qDebug()<<"右键";
    }
}





//鼠标移动事件
void test::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton){	//必须要用到buttons();
        qDebug()<<"移动";
    }
}

//滚轮事件
void test::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0){           //滚轮向我们走远
          qDebug()<<"放大";//放大
    }else{                          //滚轮向我们靠近
            qDebug()<<"缩小";//缩小
    }
}

