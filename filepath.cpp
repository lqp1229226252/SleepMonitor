#include "filepath.h"
#include "ui_filepath.h"
#include "QFileDialog"
#include "QDebug"
#include <QCoreApplication>
FilePath::FilePath(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilePath)
{
    ui->setupUi(this);
    ui->FP_Path->setEnabled(false);
    ui->light_path->setEnabled(false);
    ui->angelacc_Path->setEnabled(false);
    ui->snore_path->setEnabled(false);
    ui->gro_acc_path->setEnabled(false);

    QString exe_path=QCoreApplication::applicationDirPath();

    FP_Path=exe_path+"fp_fp.txt";
    ui->FP_Path->setText(FP_Path);
    light_Path=exe_path+"light_light.txt";
    ui->light_path->setText(light_Path);
    angle_acc_Path=exe_path+"angleacc_angleacc.txt";
    ui->angelacc_Path->setText(angle_acc_Path);
    snore_Path=exe_path+"snore_snore.txt";
    ui->snore_path->setText(snore_Path);
    gro_acc_path=exe_path+"groacc_groacc.txt";
    ui->gro_acc_path->setText(gro_acc_path);

    this->setModal(true);

}

FilePath::~FilePath()
{
    delete ui;
}

QString FilePath::getFPPath()
{
    return this->FP_Path;
}

QString FilePath::getLightPath()
{
    return this->light_Path;
}

QString FilePath::getAngleAccPath()
{
    return this->angle_acc_Path;
}

QString FilePath::getSnorePath()
{
    return this->snore_Path;
}

QString FilePath::getGroAccPath()
{
    return this->gro_acc_path;
}

void FilePath::closeEvent(QCloseEvent *e)
{
    emit(close());
}

void FilePath::on_select_FP_clicked()
{

   QString name=QFileDialog::getSaveFileName(nullptr,"选择路径","","*.txt");
   if(name.length()==0)
   {
       return;
   }
   int name_len=name.length();
   name.insert(name_len-4,"_fp");
   FP_Path=name;
   ui->FP_Path->setText(name);
   emit(PathChang(0,name));
//   qDebug()<<name;
}

void FilePath::on_select_light_clicked()
{
    QString name=QFileDialog::getSaveFileName(nullptr,"选择路径","","*.txt");
    if(name.length()==0)
    {
        return;
    }
    int name_len=name.length();
    name.insert(name_len-4,"_light");
    light_Path=name;
    ui->light_path->setText(name);
    emit(PathChang(1,name));
}

void FilePath::on_select_angle_acc_clicked()
{
    QString name=QFileDialog::getSaveFileName(nullptr,"选择路径","","*.txt");
    if(name.length()==0)
    {
        return;
    }
    int name_len=name.length();
    name.insert(name_len-4,"_angleacc");
    angle_acc_Path=name;
    ui->angelacc_Path->setText(name);
    emit(PathChang(2,name));
}

void FilePath::on_select_snore_clicked()
{
    QString name=QFileDialog::getSaveFileName(nullptr,"选择路径","","*.txt");
    if(name.length()==0)
    {
        return;
    }
    int name_len=name.length();
    name.insert(name_len-4,"_snore");
    snore_Path=name;
    ui->snore_path->setText(name);
    emit(PathChang(3,name));
}

void FilePath::on_select_gro_acc_clicked()
{
    QString name=QFileDialog::getSaveFileName(nullptr,"选择路径","","*.txt");
    if(name.length()==0)
    {
        return;
    }
    int name_len=name.length();
    name.insert(name_len-4,"_groacc");
    gro_acc_path=name;
    ui->gro_acc_path->setText(name);
    emit(PathChang(4,name));
}

