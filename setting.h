/*#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include<QPoint>
#include<QMouseEvent>
namespace Ui {
class setting;
}

class setting : public QWidget
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();
    void initialize_date(int& fun1,int& fun2);
    int  delaytime();
    //去除边框之后移动使用
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);


private slots:
    void on_pushButton_clicked();


private:
    Ui::setting *ui;
    QPoint press,origin;
    bool isdraging;
    int delay;
    bool clipboard;
    bool directsave;
    bool link;

    /*
    int delay;
    int f1,f2;};
*/


#endif // SETTING_H
