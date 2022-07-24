#ifndef FILEPATH_H
#define FILEPATH_H

#include <QDialog>
#include "QFileDialog"
namespace Ui {
class FilePath;
}

class FilePath : public QDialog
{
    Q_OBJECT

public:
    explicit FilePath(QWidget *parent = nullptr);
    ~FilePath();
    QString getFPPath();
    QString getLightPath();
    QString getAngleAccPath();
    QString getSnorePath();
    QString getGroAccPath();
    void closeEvent(QCloseEvent *);
signals:
    void PathChang(int,QString);
    void close();
private slots:
    void on_select_light_clicked();

    void on_select_angle_acc_clicked();

    void on_select_snore_clicked();

    void on_select_gro_acc_clicked();

    void on_select_FP_clicked();

private:
    Ui::FilePath *ui;
    QString FP_Path;
    QString light_Path;
    QString angle_acc_Path;
    QString snore_Path;
    QString gro_acc_path;
};

#endif // FILEPATH_H
