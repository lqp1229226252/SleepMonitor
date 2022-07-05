#include "widget.h"
#include "QObject"
#include "QByteArray"
#include "QRegExp"
#include "QRegExpValidator"
#include "QDebug"
ComboBox::ComboBox(QWidget *parent):QWidget (parent)
{

   this->vbox=new QVBoxLayout(this);
   this->label.setAlignment(Qt::AlignCenter);
   this->vbox->addWidget(&label,0,Qt::AlignCenter);
   this->vbox->addWidget(&combobox);
   connect(&combobox,SIGNAL(currentIndexChanged(int)),this,SLOT(slot(int)));
   this->setSize();
}
ComboBox::~ComboBox()
{
    delete  vbox;
}
void ComboBox::setlabel(QString Text)
{
    this->label.setText(Text);
}
void ComboBox::setItems(QStringList list)
{
    for(int i=0;i<list.length();i++)
    {
        this->combobox.addItem(list[i]);
    }
}
void ComboBox::setFontSize(QFont font)
{
    label.setFont(font);
    combobox.setFont(font);
}
void ComboBox::setSize(int w, int h)
{
    this->setFixedSize(w,h);
    this->label.setFixedSize(int(w*0.9),int(h*0.3));
    this->combobox.setFixedSize(int(w*0.9),int(h*0.4));
}
void ComboBox::setCurrentIndex(int index)
{
    this->combobox.setCurrentIndex(index);
}
void ComboBox::slot(int index)
{
    emit(currentIndexChanged(index));
}

CheckBoxList::CheckBoxList(QWidget *parent):QWidget (parent)
{
    this->grid=new QGridLayout(this);
    this->groupbutton.setExclusive(false);
    this->grid->setContentsMargins(0,0,0,0);
    this->grid->setSpacing(0);
    this->length=0;
    connect(&groupbutton,SIGNAL(buttonClicked(int)),this,SLOT(slot(int)));
}
CheckBoxList::~CheckBoxList()
{
    qDeleteAll(labellist);
    qDeleteAll(checkboxlist);
    qDeleteAll(labellist);
    delete grid;
}
void CheckBoxList::create(QStringList labels, bool flag)
{
    length=labels.length();
    if(flag)
    {
       QLabel *label=new QLabel;
       this->grid->addWidget(label,0,0);
       this->labelempty.append(label);
       for(int i=0;i<length;i++)
       {
           QCheckBox *checkbox=new QCheckBox();
           QLabel  *label=new QLabel(labels[i]);
           label->resize(checkbox->width(),checkbox->height()*2);
           this->grid->addWidget(label,0,i+1,Qt::AlignCenter);
           this->grid->addWidget(checkbox,1,i+1,Qt::AlignCenter);
           this->groupbutton.addButton(checkbox,i);
           labellist.append(label);
           checkboxlist.append(checkbox);
       }
        for(int i=0;i<3;i++)
        {
            QLabel *label=new QLabel;
            this->grid->addWidget(label,0,this->length+i+1);
            this->labelempty.append(label);
        }
    }
    else
    {
        for(int i=0;i<length;i++)
        {
            QCheckBox *checkbox=new QCheckBox();
            QLabel  *label=new QLabel(labels[i]);
            label->resize(checkbox->width(),checkbox->height()*2);
            this->grid->addWidget(label,0,i,Qt::AlignCenter);
            this->grid->addWidget(checkbox,1,i,Qt::AlignCenter);
            this->groupbutton.addButton(checkbox,i);
            labellist.append(label);
            checkboxlist.append(checkbox);
        }
    }

}

void CheckBoxList::setFontSize(QFont font)
{
    for(int i=0;i<this->length;i++)
    {
//        this->labels[i].setFont(font);
    }
}
void CheckBoxList::slot(int index)
{
    QByteArray buffer;
    uint8_t data1=0;
    uint8_t data2=0;
    for(int i=0;i<this->length;i++)
    {
        QCheckBox *checkbox=(QCheckBox *)this->groupbutton.button(i);
        if(checkbox->checkState())
           if(i<8)
              { data1+=1<<(7-i);}
           else
              {data2+=1<<(15-i);}
    }
    buffer.append(char(data1));
    buffer.append(char(data2));
    emit(check(buffer));

}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->hbox1=new QHBoxLayout();

    this->collect_vbox=new QVBoxLayout();
    this->point_vbox=new QVBoxLayout();
    this->hbox2=new QHBoxLayout();
    this->hbox2->addLayout(collect_vbox);
    this->hbox2->addLayout(point_vbox);

    this->vbox1=new QVBoxLayout();
    this->vbox1->addLayout(hbox1);
    this->vbox1->addLayout(hbox2);

    this->send_vbox=new QVBoxLayout();
    this->hbox=new QHBoxLayout(this);
    this->hbox->addLayout(vbox1);
    this->hbox->addLayout(send_vbox);
    setUI();
    setConnect();
    initBuf();
    setFontSize(8);
    setSize(700,200);
}

Widget::~Widget()
{
    delete hbox;
    delete  hbox1;
    delete  hbox2;
    delete  collect_vbox;
    delete  point_vbox;
    delete  vbox1;
    delete  send_vbox;

}
void Widget::initBuf()
{
   buf_head=0xBE;
   buf_len=0x0D;
   buf_code=0x02;
   buf_collect_contral=0xAA;
   buf_collect_rate=0xA0;
   buf_collect_res_high=0x00;
   buf_collect_res_low=0x00;
   buf_num=0;
   buf_end=0xEB;
}

void Widget::setUI()
{

     //数据采样控制UI
    this->collect_control_ui.setlabel("采集控制");
    this->collect_control_ui.setItems({"连续数据采集","采集一包数据","关闭采集","采样特定点数的数据"});
    this->brain_collect_rate_ui.setlabel("脑电采样率MSB");
    this->brain_collect_rate_ui.setItems({"100sps","200sps","500sps","800sps"});
    this->brain_collect_rate_ui.setCurrentIndex(2);
    this->snoring_collect_rate_ui.setlabel("鼾声采样率");
    this->snoring_collect_rate_ui.setItems({"200sps","500sps","1000sps","1600sps"});
    this->snoring_collect_rate_ui.setCurrentIndex(2);
    this->brainoxygen_collect_rate_ui.setlabel("脑氧采样率");
    this->brainoxygen_collect_rate_ui.setItems({"100sps","200sps","300sps","400sps"});
    this->body_action_rate_ui.setlabel("体动采样率");
    this->body_action_rate_ui.setItems({"100sps","500sps","1000sps","1600sps"});
    this->hbox1->setContentsMargins(0,0,0,0);
    this->hbox1->setSpacing(1);
    this->hbox1->addWidget(&collect_control_ui);
    this->hbox1->addWidget(&brain_collect_rate_ui);
    this->hbox1->addWidget(&snoring_collect_rate_ui);
    this->hbox1->addWidget(&brainoxygen_collect_rate_ui);
    this->hbox1->addWidget(&body_action_rate_ui);

    //采集数据源UI
    this->collect_label.setText("采样数据源");
    QStringList collect1_label={"FP1","FP2","红光","近红光","绿光","坐立角","翻滚角","运动\n加速度","左声道\n鼾声","右声道\n鼾声"};
    this->collect1.create(collect1_label);
    QStringList  collect2_label={"x-gro","y-gro","z-gro","x-acc","y-acc","z-acc"};
    this->collect2.create(collect2_label,true);
    this->collect_vbox->addWidget(&collect_label,1,Qt::AlignCenter);
    this->collect_vbox->addWidget(&collect1,2);
    this->collect_vbox->addWidget(&collect2,2);
    this->collect_vbox->addStretch(1);

    //采集点数
    this->collect_point_label.setText("采样点数");
    QRegExp re("[0-9]{1,5}");
    QRegExpValidator *validator=new QRegExpValidator(re);
    this->collect_point.setValidator(validator);
    this->collect_point.setText("1000");
    this->point_vbox->addStretch(2);
    this->point_vbox->addWidget(&collect_point_label,1,Qt::AlignCenter);
    this->point_vbox->addWidget(&collect_point,1);
    this->point_vbox->addStretch(2);

//    发送按钮
    this->send_btn.setText("发送\n指令");
    this->send_vbox->addStretch(1);
    this->send_vbox->addWidget(&send_btn,5);
    this->send_vbox->addStretch(1);
}

void Widget::setConnect()
{
    connect(&collect_control_ui,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot1(int)));
    connect(&brain_collect_rate_ui,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot2(int)));
    connect(&snoring_collect_rate_ui,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot3(int)));
    connect(&brainoxygen_collect_rate_ui,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot4(int)));
    connect(&body_action_rate_ui,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxSlot5(int)));
    connect(&collect1,SIGNAL(check(QByteArray)),this,SLOT(checkSlot1(QByteArray)));
    connect(&collect2,SIGNAL(check(QByteArray)),this,SLOT(checkSlot2(QByteArray)));
    connect(&send_btn,SIGNAL(clicked()),this,SLOT(sendSlot()));
}

void Widget::comboBoxSlot1(int index)
{
     if(index==0)
         this->buf_collect_contral=0xaa;
     if(index==1)
         this->buf_collect_contral=0xbb;
     if(index==2)
         this->buf_collect_contral=0xCC;
     if(index==3)
         this->buf_collect_contral=0xDD;
}

void Widget::comboBoxSlot2(int index)
{
    this->buf_collect_rate&=0x3F;
    this->buf_collect_rate+=(index<<6);
}

void Widget::comboBoxSlot3(int index)
{
    this->buf_collect_rate&=0XCF;
    this->buf_collect_rate+=(index<<4);
}
void Widget::comboBoxSlot4(int index)
{
    this->buf_collect_rate&=0XF3;
    this->buf_collect_rate+=(index<<2);
}
void Widget::comboBoxSlot5(int index)
{
    this->buf_collect_rate&=0XFC;
    this->buf_collect_rate+=index;
}

void Widget::checkSlot1(QByteArray buffer)
{
    this->buf_collect_res_high=uint8_t(buffer.at(0));
    uint8_t temp=this->buf_collect_res_low&0x3F;
    this->buf_collect_res_low=uint8_t(buffer.at(1))+temp;
}
void Widget::checkSlot2(QByteArray buffer)
{
    uint8_t temp=this->buf_collect_res_low&0xC0;
    this->buf_collect_res_low=temp+(uint8_t(buffer.at(0))>>2);
//    qDebug()<<this->buf_collect_res_low;
}
void Widget::sendSlot()
{
    QByteArray buffer;
    this->buf_num++;
    uint16_t buf_collect_point=uint16_t(this->collect_point.text().toUInt());
    //#帧头
    buffer.append(char(this->buf_head));
    //      #帧长
    buffer.append((char(this->buf_len&0xFF00)>>8));
    buffer.append((char(this->buf_len&0xFF)));
    //      #功能码
    buffer.append(char(this->buf_code));
    //      #数据
    buffer.append(char(this->buf_collect_contral));//#采样控制
    buffer.append(char(this->buf_collect_rate));//#采样率
    buffer.append(char(this->buf_collect_res_high));//#采样源
    buffer.append(char(this->buf_collect_res_low));
    buffer.append(char(buf_collect_point>>8));//#采样数据点数
    buffer.append(char(buf_collect_point));
    //校验码
    buffer.append(char(cal_verify(buf_collect_point)));
    //#帧序
    buffer.append(char(this->buf_num));
//   #帧尾
    buffer.append(char(this->buf_end));
    qDebug()<<buffer;
    emit(sendSingal(buffer));

}
uint8_t Widget::cal_verify(uint16_t data)
{
   int sum=this->buf_collect_contral;
   sum+=this->buf_collect_rate;
   sum+=(this->buf_collect_res_high<<8);
   sum+=this->buf_collect_res_low;
   sum+=data;
   uint8_t verify=sum&0xFF;
   return verify;
}

void Widget::setFontSize(int size)
{
   QFont font;
   font.setPointSize(size);
   this->collect_control_ui.setFontSize(font);
   this->brain_collect_rate_ui.setFontSize(font);
   this->snoring_collect_rate_ui.setFontSize(font);
   this->brainoxygen_collect_rate_ui.setFontSize(font);
   this->body_action_rate_ui.setFontSize(font);
   this->collect1.setFontSize(font);
   this->collect2.setFontSize(font);
}

void Widget::setSize(int w, int h)
{
    if(w<600)
       w=600;
    if(h<200)
       h=200;
    this->setFixedSize(w,h);
    this->collect_control_ui.setSize(w*0.2,h*0.3);
    this->brain_collect_rate_ui.setSize(w*0.18,h*0.3);
    this->snoring_collect_rate_ui.setSize(w*0.15,h*0.3);
    this->brainoxygen_collect_rate_ui.setSize(w*0.15,h*0.3);
    this->body_action_rate_ui.setSize(w*0.15,h*0.3);
    this->send_btn.setFixedSize(w*0.12,h*0.8);
    this->collect_point.setFixedSize(w*0.15,h*0.15);
}





