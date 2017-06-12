/*#include "setting.h"
#include "ui_setting.h"
#include<QFile>
#include<QTextStream>
#include<QIODevice>
#include<QDebug>
setting::setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting)
{
     this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明2-窗体标题栏不透明,背景透明
      this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QFile data("save.txt");
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);
        int delay2,clipboard2,directsave2,link2;
      in>>delay2>>clipboard2>>directsave2>>link2;\
      delay=delay2;
      clipboard=clipboard2;
      directsave=directsave2;
      link=link2;
      qDebug()<<link2;

    }
    else qDebug()<<"NO";

    ui->setupUi(this);
    ui->spinBox->setValue(delay);
    ui->checkBox->setChecked(clipboard);
    ui->radioButton->setChecked(directsave);
    ui->radioButton_2->setChecked(link);

    /*
    delay=0;
    f1=f2=0;
*//*
}

setting::~setting()
{
    /*
      if(this!=nullptr)
          delay=ui->spinBox->value();
      if(ui->checkBox->isChecked())
          f1=2;    //剪切功能 ，fun1=2,否则为0;
      //fun2为1表示直接截图，为3表示连接到图片程序，否则为0
      if(ui->radioButton->isChecked())
          f2=1;
      if(ui->radioButton_2->isChecked())
          f2=3;
*/
/*
    delete ui;
}

 void setting::initialize_date(int& fun1,int&fun2)
 {
     if(ui->checkBox->isChecked())
         fun1=2;    //剪切功能 ，fun1=2,否则为0;
     //fun2为1表示直接截图，为3表示连接到图片程序，否则为0
     if(ui->radioButton->isChecked())
         fun2=1;
     if(ui->radioButton_2->isChecked())
         fun2=3;

 }
int  setting::delaytime()
{
    return ui->spinBox->value();
}

void setting::on_pushButton_clicked()
{
    delay=ui->spinBox->value();
    if(ui->checkBox->isChecked())
        clipboard=true;
    else clipboard=false;
    if(ui->radioButton->isChecked())
        directsave=true;
    else directsave=false;
    if(ui->radioButton_2->isChecked())
         link=true;
    else link=false;

    QFile data("save.txt");
    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&data);
        out <<delay<<" "<<clipboard<<" "<<directsave<<" "<<link;
    }
    this->close();
}

/*
void setting::mousePressEvent(QMouseEvent *e)
{
    isdraging=true;
   press=e->globalPos();
    origin=this->pos();


}
/*
void setting::mouseReleaseEvent(QMouseEvent *e)
{
    isdraging=false;
}


void setting::mouseMoveEvent(QMouseEvent *e)
{
    if(isdraging)
    {
        int x=origin.x()+e->globalPos().x()-press.x();
        int y=origin.y()+e->globalPos().y()-press.y();
       this->move(x,y);
    }

}

*/
