#include "outputwnd.h"
#include "ui_outputwnd.h"

OutPutWnd::OutPutWnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OutPutWnd)
{
    ui->setupUi(this);
}

void OutPutWnd::OutPut(QString str){
    ui->textBrowser->append(str);
}


OutPutWnd::~OutPutWnd()
{
    delete ui;
}

