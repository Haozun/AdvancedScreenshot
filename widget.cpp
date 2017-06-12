#include "widget.h"
#include "ui_widget.h"
#include<QIODevice>
#include<QDesktopServices>
#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    setAutoStart(true);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect screenRect = desktopWidget->screenGeometry();
    g_nActScreenX = screenRect.width();
    g_nActScreenY = screenRect.height();
    QxtGlobalShortcut* allsc = new QxtGlobalShortcut(QKeySequence(tr("Alt+T")), this);
    connect(allsc, SIGNAL(activated()),this, SLOT(alltoggle()));
    QxtGlobalShortcut* allcamera = new QxtGlobalShortcut(QKeySequence(tr("Alt+C")), this);
    connect(allcamera, SIGNAL(activated()),this, SLOT(on_pushButton_10_clicked()));
    QxtGlobalShortcut* allrec = new QxtGlobalShortcut(QKeySequence(tr("Alt+R")), this);
    connect(allrec, SIGNAL(activated()),this, SLOT(on_RectpushButton_clicked()));
    QxtGlobalShortcut* allellipse = new QxtGlobalShortcut(QKeySequence(tr("Alt+P")), this);
    connect(allellipse, SIGNAL(activated()),this, SLOT(on_pushButton_3_clicked()));
    QxtGlobalShortcut* allall = new QxtGlobalShortcut(QKeySequence(tr("Alt+F")), this);
    connect(allall, SIGNAL(activated()),this, SLOT(on_FullpushButton_clicked()));
    //this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明2-窗体标题栏不透明,背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    ui->setupUi(this);




    mCamera = new QCamera;//启用系统摄像头
    mCameraImageCapture = new QCameraImageCapture(mCamera);//用摄像头初始化一个QCameraImageCapture用来截图
    mCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    mCamera->setCaptureMode(QCamera::CaptureStillImage);//设置摄像头的模式，可以抓取静态图像





    //延时

    delay=ui->spinBox->value();

    QFile data("save.txt");
    int delay2,clipboard2,directsave2,link2,drag2,isobscure;
    QString path;
    if(data.exists())
    {

        if (data.open(QFile::ReadOnly))
        {
            QTextStream in(&data);

            in>>delay2>>clipboard2>>directsave2>>link2>>drag2>>isobscure>>path;

        }
    }
    else
    {
        delay2=0;
        clipboard2=1;
        directsave2=0;
        link2=1;
        drag2=0;
        isobscure=0;
        //获取当前路径
        QDir dir;
        path=dir.currentPath();

    }

    ui->spinBox->setValue(delay2);
    ui->checkBox->setChecked(clipboard2);
    ui->radioButton->setChecked(link2);
    ui->radioButton_2->setChecked(directsave2);
    ui->checkBox_2->setChecked(drag2);
    ui->checkBox_3->setChecked(ishidemode);
    ui->checkBox_4->setChecked(issuicide);
    ui->checkBox_5->setChecked(isobscure);
    ui->lineEdit->setText(path);

    this->rolledOutDown = false;
    shotway=-1;

    this->defaultHeight = this->height();
    this->defaultWidth = this->width();


    mSysTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/trayicon/images/fileico.png");//托盘图片
    mSysTrayIcon->setIcon(icon); //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setToolTip(QObject::tr("SSR截图"));    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    mSysTrayIcon->show();


    createActions();
    createMenu();    //建立菜单




}

Widget::~Widget()
{
    delete ui;
}

//椭圆截图

void Widget::on_RoundpushButton_clicked()
{
    if(RectpushButton_clicked==true)
    {
    RectpushButton_clicked=false;
    shotway=2;
    delay=ui->spinBox->value();
    //延时
    this->timer=new QTimer;
    RectpushButton_clicked=true;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(shotScreenSlot()));
    this->timer->start(delay*1000+1);
    }
}

//矩形截图button
void Widget::on_RectpushButton_clicked()
{
    if(RectpushButton_clicked==true)
    {
    RectpushButton_clicked=false;
    delay=ui->spinBox->value();
    shotway=0;
    this->timer=new QTimer;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(shotScreenSlot()));
    this->timer->start(delay*1000+1);
    }
}
//全屏截图button
void Widget::on_FullpushButton_clicked()
{    if(RectpushButton_clicked==true)
    {
    RectpushButton_clicked=false;
    delay=ui->spinBox->value();
    shotway=1;

    this->timer=new QTimer;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(shotScreenSlot()));
    RectpushButton_clicked=true;
    this->timer->start(delay*1000+1);
    }
}

//多边形
void Widget::on_pushButton_3_clicked()
{if(RectpushButton_clicked==true)
    { RectpushButton_clicked=false;
    delay=ui->spinBox->value();
    shotway=3;
    this->timer=new QTimer;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(shotScreenSlot()));
    this->timer->start(delay*1000+100);
    }
}

//真正截图功能实现truescreen
void Widget::shotScreenSlot()
{
    this->hide();
    int fun1=0,fun2=0;
    bool ifdrag=true;
    bool mini=isminimize;//废弃
    int isblur=0;



    if(ui->checkBox_4->isChecked())
        issuicide=1;
    if(!ui->checkBox_2->isChecked())
        ifdrag=false;
    if(ui->checkBox->isChecked())
        fun1=2;
    if(ui->checkBox_5->isChecked())
        isblur=1;
    if(ui->radioButton_2->isChecked())
        fun2=1;
    if(ui->radioButton->isChecked())
        fun2=3;
    QString path=ui->lineEdit->text();
    //全屏截图
    if(shotway==1)
    {
        temp=QPixmap::grabWindow(QApplication::desktop()->winId());
        pixmap=new QPixmap(temp);
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("MM.dd-hh.mm.ss");
        //直接保存到文件管理
        if(fun2==1)
        {

            // 保存文件全路径
            str = ui->lineEdit->text() + "/" + str+ ".png";
            // 保存
            pixmap->save(str, "png");

        }
        //连接到图片程序
        if(fun2==3)
        {

            // 保存文件全路径
            str = ui->lineEdit->text() + "/" + str+ ".png";
            // 保存
            pixmap->save(str, "png");
            QString picPath = "file:///" + str;
            QDesktopServices::openUrl(QUrl(picPath));


        }


        //把文件放到剪切板上
        if(fun1==2)
            QApplication::clipboard()->setImage(pixmap->toImage());

        if(isminimize==false){this->show();}

    }
    else{
        mSysTrayIcon->hide();
        this->setWindowFlags(this->windowFlags()&~ Qt::WindowStaysOnTopHint);
        //默认矩形截图 index=0
        int temp=0;
        //index为2时为圆形截图，为3时为多边形截图
        if(shotway==2)
            temp=2;
        if(shotway==3)
            temp=3;
        if(issuicide!=0)
            temp=9;
        if(shotway==8)
            temp=8;
        shot = new FullScreen(temp);
        RectpushButton_clicked=true;
        shot->refun(fun1,fun2,ifdrag,path,mini,isblur);
    }

    if(issuicide==0)
    {
         this->timer->stop();
    }

    if(isminimize==false)
    {
        this->show();
    }
    mSysTrayIcon->show();//没有逻辑,全靠一次次实验

}


//保存button
void Widget::on_savepushButton_clicked()
{
    if(shotway!=-1)
    {
        if(shotway!=1)
            pixmap=new QPixmap(shot->reshot());
        //直接跳转到文件管理
        //文件路径示例   /home/Qt_picture/new.png
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("MM.dd-hh.mm.ss");
        QString picName = str;// //有修改,防bug标记
        QString filename = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存图片"),
                                                        ui->lineEdit->text()+ "/"+picName,
                                                        tr("Images (*.png *.jpg *.bmp *.xpm)"));
        if(!filename.isEmpty ())
        {
            pixmap->save(filename);
        }
    }
}

//最小化button，最小化到托盘
void Widget::on_pushButton_clicked()
{
    isminimize=true;
    mSysTrayIcon->show();
    this->hide();
}
//托盘响应
void Widget::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        alltoggle();
        break;
        //case QSystemTrayIcon::DoubleClick:(备用的双击功能,以及单击,鼠标中键)
        //this->show();
        //break;
    default:break;
    }
}
//托盘右键菜单slot
void Widget::createActions()
{

    mShowMainAction = new QAction(QObject::trUtf8("切换界面(Alt+T)"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(on_showMainAction()));
    mRecAction =new QAction(QObject::trUtf8("矩形(Alt+R)"),this);
    connect(mRecAction,SIGNAL(triggered()),this,SLOT(on_RectpushButton_clicked()));
    mEllipseAction =new QAction(QObject::tr("椭圆"),this);
    connect(mEllipseAction,SIGNAL(triggered()),this,SLOT(on_RoundpushButton_clicked()));
    mFullAction =new QAction(QObject::tr("全屏(Alt+F)"),this);
    connect(mFullAction,SIGNAL(triggered()),this,SLOT(on_FullpushButton_clicked()));
    mCameraAction =new QAction(QObject::trUtf8("snap(Alt+C)"),this);
    connect(mCameraAction,SIGNAL(triggered()),this,SLOT(on_pushButton_10_clicked()));
    mExitAppAction = new QAction(QObject::tr(" X   "),this);
    informationAction = new QAction(QObject::tr("@华中科技大学软件学院"),this);
    informationAction2 = new QAction(QObject::tr("@专业实训展示版v10.0"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
    mArbitraryAction = new QAction(QObject::tr("任意截图(Alt+P)"),this);
    connect(mArbitraryAction,SIGNAL(triggered()),this,SLOT(on_pushButton_3_clicked()));
}
//托盘右键菜单menu
void Widget::createMenu()
{
    mMenu = new QMenu(this);

    mMenu->addAction(mFullAction);
    mMenu->addAction(mRecAction);
    mMenu->addAction(mArbitraryAction);
    mMenu->addAction(mEllipseAction);
    mMenu->addSeparator();
    mMenu->addAction(mCameraAction);
    mMenu->addAction(informationAction);
    mMenu->addAction(informationAction2);
    mMenu->addSeparator();
    mMenu->addAction(mShowMainAction);
    mMenu->addAction(mExitAppAction);

    mSysTrayIcon->setContextMenu(mMenu);
}

void Widget::on_showMainAction()
{
    isminimize=false;

    this->show();
}

void Widget::on_exitAppAction()
{
    mSysTrayIcon->hide();
    exit(0);

}


//退出button
void Widget::on_closepushButton_clicked()
{

    int delay;
    delay=ui->spinBox->value();
    int clipboard,directsave,link,drag,isobscure;

    if(ui->checkBox->isChecked())
        clipboard=true;
    else clipboard=false;
    if(ui->radioButton->isChecked())
        link=true;
    else link=false;
    if(ui->radioButton_2->isChecked())
        directsave=true;
    else directsave=false;
    if(ui->checkBox_2->isChecked())
        drag=true;
    else drag=false;
    if(ui->checkBox_5->isChecked())
        isobscure=true;
    else isobscure=false;
    QString path=ui->lineEdit->text();
    QFile data("save.txt");
    if (data.open(QFile::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&data);
        out <<delay<<" "<<clipboard<<" "<<directsave<<" "
           <<link<<" "<<drag<<" "<<isobscure<<" "<<path;
    }

    mSysTrayIcon->hide();
    exit(0);
}

//设置button
void Widget::on_pushButton_2_clicked()
{

}
//切换主菜单
void Widget::alltoggle()
{
    if(isVisible())
    {
        on_pushButton_clicked();
    }
    else
    {
        on_showMainAction();
        if(!isVisible())
        {
            on_showMainAction();
        }
    }

}


void Widget::mousePressEvent(QMouseEvent *e)
{
    isdraging=true;
    press=e->globalPos();
    origin=this->pos();
    if(isattractwidget==true)
    {

        QPoint temppoint=this->pos();
        int x=temppoint.x();
        int y=temppoint.y();
        if(e->globalPos().y()<20&&
                 ((e->globalPos().x()>x)&&(e->globalPos().x()<x+defaultWidth)))

        {
            this->move(x,0);unfoldrightwards();isattractwidget=false;
        }


        else if ((e->globalPos().x()>(g_nActScreenX-20))&&
                 (e->globalPos().y()>y)&&(e->globalPos().y()<y+defaultHeight))
        {
            this->move(g_nActScreenX-defaultWidth,y);
            unfoldupwards();
            isattractwidget=false;
        }
    }

}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{Q_UNUSED(e)
            isdraging=false;
}


void Widget::mouseMoveEvent(QMouseEvent *e)
{    if(isdraging&&isattractwidget==false)
    {
        int x=origin.x()+e->globalPos().x()-press.x();
        int y=origin.y()+e->globalPos().y()-press.y();
        this->move(x,y);
    }
    //吸附窗口
    {//因为默认isdraging才会监控
        if(isattractwidget==false)
        {
            QPoint temppoint=this->pos();
             if(temppoint.y()<-10)
            {
                this->move(temppoint.x(),-325);
                foldrightwards();isattractwidget=true;setWindowFlags(this->windowFlags()| Qt::WindowStaysOnTopHint);this->show();
            }
            else if (temppoint.x()>(g_nActScreenX-defaultWidth+10))
            {
                this->move(g_nActScreenX-20,temppoint.y());
                foldupwards();isattractwidget=true;this->setWindowFlags(this->windowFlags()| Qt::WindowStaysOnTopHint);this->show();
            }
        }

     }

}



//向下弹框动画
void Widget::toggleDown()
{
    if (this->rolledOutDown)
    {
        this->rollInDown();
    }
    else
    {
        this->rollOutDown();
    }

}

//窗口吸附区域,284行,几乎都是多余

void Widget::unfoldrightwards()
{
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumWidth");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumWidth");
    animationMin->setDuration(150);
    animationMin->setStartValue(geometry.width());
    animationMin->setEndValue(defaultWidth);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(150);
    animationMax->setStartValue(geometry.width());
    animationMax->setEndValue(defaultWidth);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::unfoldupwards()
{
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumHeight");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumHeight");
    animationMin->setDuration(150);
    animationMin->setStartValue(geometry.height());
    animationMin->setEndValue(defaultHeight);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(150);
    animationMax->setStartValue(geometry.height());
    animationMax->setEndValue(defaultHeight);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::foldrightwards(int width)
{
    QRect geometry = this->geometry();
    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumWidth");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumWidth");
    animationMin->setDuration(150);
    animationMin->setStartValue(geometry.width());
    animationMin->setEndValue(width);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(150);
    animationMax->setStartValue(geometry.width());
    animationMax->setEndValue(width);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}
void Widget::foldupwards(int height)
{
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumHeight");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumHeight");
    animationMin->setDuration(150);
    animationMin->setStartValue(geometry.height());
    animationMin->setEndValue(height);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(150);
    animationMax->setStartValue(geometry.height());
    animationMax->setEndValue(height);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::rollInDown()
{

    this->rolledOutDown = false;
    this->rolledOutDownHelp = false;
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumHeight");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumHeight");
    animationMin->setDuration(500);
    animationMin->setStartValue(geometry.height());
    animationMin->setEndValue(this->defaultHeight);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(500);
    animationMax->setStartValue(geometry.height());
    animationMax->setEndValue(this->defaultHeight);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::rollInRight()
{

    this->rolledOutRight = false;
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumWidth");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumWidth");
    animationMin->setDuration(600);
    animationMin->setStartValue(geometry.width());
    animationMin->setEndValue(this->defaultWidth);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(600);
    animationMax->setStartValue(geometry.width());
    animationMax->setEndValue(this->defaultWidth);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::rollOutRight(int width)
{

    this->rolledOutRight = true;
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumWidth");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumWidth");
    animationMin->setDuration(600);
    animationMin->setStartValue(geometry.width());
    animationMin->setEndValue(width);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(600);
    animationMax->setStartValue(geometry.width());
    animationMax->setEndValue(width);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::rollOutDown(int height)
{

    this->rolledOutDown = true;
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumHeight");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumHeight");
    animationMin->setDuration(1000);
    animationMin->setStartValue(geometry.height());
    animationMin->setEndValue(height);
    animationMin->setEasingCurve(QEasingCurve::OutExpo);
    animationMax->setDuration(1000);
    animationMax->setStartValue(geometry.height());
    animationMax->setEndValue(height);
    animationMax->setEasingCurve(QEasingCurve::OutExpo);
    animationMin->start();
    animationMax->start();
}

void Widget::rollOutDownHelp(int height)
{

    this->rolledOutDown = true;
    this->rolledOutDownHelp = true;
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumHeight");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumHeight");
    animationMin->setDuration(500);
    animationMin->setStartValue(geometry.height());
    animationMin->setEndValue(height);
    animationMin->setEasingCurve(QEasingCurve::OutBounce);
    animationMax->setDuration(500);
    animationMax->setStartValue(geometry.height());
    animationMax->setEndValue(height);
    animationMax->setEasingCurve(QEasingCurve::OutBounce);
    animationMin->start();
    animationMax->start();
}

void Widget::rollOutDownhide(int height)
{

    this->rolledOutDown = true;
    this->rolledOutDownHelp= true;
    QRect geometry = this->geometry();

    QPropertyAnimation *animationMin = new QPropertyAnimation(this, "minimumHeight");
    QPropertyAnimation *animationMax = new QPropertyAnimation(this, "maximumHeight");
    animationMin->setDuration(500);
    animationMin->setStartValue(geometry.height());
    animationMin->setEndValue(height);
    animationMin->setEasingCurve(QEasingCurve::OutBounce);
    animationMax->setDuration(500);
    animationMax->setStartValue(geometry.height());
    animationMax->setEndValue(height);
    animationMax->setEasingCurve(QEasingCurve::OutBounce);
    animationMin->start();
    animationMax->start();
}

//蓝屏假象
void Widget::on_pushButton_5_clicked()
{
    shotway=8;
    this->timer=new QTimer;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(shotScreenSlot()));
    this->timer->start(delay*1000+1);
}

void Widget::on_saveDirpushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    ui->lineEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(dir);
}
//帮助的按钮
void Widget::on_pushButton_6_clicked()
{
    if(rolledOutDownHelp == false)
    {
        if(ui->checkBox_3->isChecked())
        {
            this->rollOutDownhide();
        }
        else rollOutDownHelp();
    }
    else rollInDown();
}

//废弃
void Widget::refun2(bool minimizeFulls)
{
    isminimize=minimizeFulls;
}

//先睹为快
void Widget::on_pushButton_4_clicked()
{
    shotway=0;
    this->timer=new QTimer;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(shotScreenSlot()));
    this->timer->start(0);
}
//网页链接
void Widget::on_pushButton_8_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://screenshot.net/zh/free-image-uploader.html")));
}

void Widget::on_pushButton_9_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("http://chuantu.biz")));
}

void Widget::on_pushButton_10_clicked()
{
    this->timer=new QTimer;
    QObject::connect(this->timer,SIGNAL(timeout()),SLOT(camerashot()));
    this->timer->start(delay*1000+1);
}
void Widget::camerashot()
{this->timer->stop();
    if(!rolledOutRight){
        if(iscamera)
        {iscamera=false;
            mCamera->start();//摄像头启动
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("MM.dd-hh.mm.ss");
            str = ui->lineEdit->text() + "/" + "CAMERA"+str+ ".png";
            mCameraImageCapture->capture(str);
            this->timer=new QTimer;
            this->timer->start(1000);
            QObject::connect(this->timer,SIGNAL(timeout()),SLOT(aaa()));
        }
        }
        else{QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("MM.dd-hh.mm.ss");
            str = ui->lineEdit->text() + "/" + "CAMERA"+str+ ".png";
            mCameraImageCapture->capture(str);}
}

void Widget::aaa()
{
    mCamera->stop();
    this->timer->stop();
    iscamera=true;
}

void Widget::on_pushButton_11_clicked()
{       mCamera->setViewfinder(ui->viewfinder);
        if (rolledOutRight)
        {
            mCamera->stop();
            this->rollInRight();
        }
        else
        { mCamera->start();
            this->rollOutRight();
        }

}

void Widget::setAutoStart(bool is_auto_start)
{
    QString application_name = QApplication::applicationName();
    QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
    if(is_auto_start)
    {
        QString application_path = QApplication::applicationFilePath();
        settings->setValue(application_name, application_path.replace("/", "\\"));
    }
    else
    {
        settings->remove(application_name);
    }
    delete settings;
}


/*
bool Widget::event(QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if (keyEvent->key() == Qt::Key_Escape) {
            qDebug() << "You press tab.";
            return true;
        }
    }
    return QWidget::event(e);
}
*/
