#include "fullscreen.h"
//#include "widget.h"
FullScreen::FullScreen(QLabel *parent) : QLabel(parent)
{      
}


//初始化截屏方式及截屏相关数据
FullScreen::FullScreen(int ind)
{
this->setAttribute(Qt::WA_DeleteOnClose);
    literalbutton=new QPushButton(this);
    literalbutton->setStyleSheet("border-image: url(:/trayicon/images/typing.png) ;");
    literalbutton->setGeometry(QRect(QPoint(-100, -100),
                  QSize(33, 22)));
    texttype=new QLineEdit(this);
     texttype->setStyleSheet("background-color: rgb(255, 255, 255,0);");

    pMenu = new QMenu(this);
    texttype->move(-100,-100);
    connect(literalbutton, SIGNAL(released()), this, SLOT(on_literalButton_clicked()));
    m_addAction1 = new QAction(QIcon(":/trayicon/images/black.png"),tr("黑色"),pMenu);
    connect(m_addAction1,SIGNAL(triggered()),this,SLOT(color1()));
    m_addAction2 = new QAction(QIcon(":/trayicon/images/red.png"),tr("红色"),pMenu);
    connect(m_addAction2,SIGNAL(triggered()),this,SLOT(color2()));
    m_addAction3 = new QAction(QIcon(":/trayicon/images/blue.png"),tr("蓝色"),pMenu);
    pMenu->addAction(m_addAction1);
    pMenu->addAction(m_addAction2);
    pMenu->addAction(m_addAction3);
    connect(m_addAction3,SIGNAL(triggered()),this,SLOT(color3()));

    // 按钮
    pButton = new QPushButton(this);

    // 设置菜单
    pButton->setMenu(pMenu);
    pButton->setGeometry(QRect(QPoint(-100, -100),
                    QSize(30, 32)));
    pButton->setStyleSheet("border-image: url(:/trayicon/images/Color.png) ;");

    tMenu = new QMenu(this);
    t_addAction1 = new QAction(QIcon(":/trayicon/images/1px.png"),tr("1px"),pMenu);
    connect(t_addAction1,SIGNAL(triggered()),this,SLOT(thick1()));
    t_addAction2 = new QAction(QIcon(":/trayicon/images/3px.png"),tr("3px"),pMenu);
    connect(t_addAction2,SIGNAL(triggered()),this,SLOT(thick2()));
    t_addAction3 = new QAction(QIcon(":/trayicon/images/6px.png"),tr("6px"),pMenu);
    tMenu->addAction(t_addAction1);
    tMenu->addAction(t_addAction2);
    tMenu->addAction(t_addAction3);
    connect(t_addAction3,SIGNAL(triggered()),this,SLOT(thick3()));

    // 按钮
    tButton = new QPushButton(this);

    // 设置菜单
    tButton->setMenu(tMenu);
    tButton->setGeometry(QRect(QPoint(-100, -100),
                  QSize(40, 22)));
    tButton->setStyleSheet("border-image: url(:/trayicon/images/penthickness.png) ;");




    A4Button= new QPushButton(tr(" "), this);

    A4Button->move(-100,-100);

    A4Button->setStyleSheet("border-image: url(:/trayicon/images/A4.png) ;");
    connect(A4Button, SIGNAL(released()), this, SLOT(changewinISO()));

    editwinbutton= new QRadioButton(tr("继续拖动窗口"), this);
    editwinbutton->move(-100,-100);
    restbutton = new QPushButton(tr("重新绘制"), this);
    restbutton->move(-100,-100);
    connect(restbutton, SIGNAL(released()), this, SLOT(resetpixmap()));
    restbutton->setStyleSheet("background-color:red;color: blue;");

    waggerybutton=new QPushButton(" ",this);
    waggerybutton->move(-100,100);
    //waggerybutton->setStyleSheet("border-image: url(:/trayicon/images/trans.png) ;");
    waggerybutton->setStyleSheet("QPushButton:hover{border-image: url(:/trayicon/images/500px.png);}"
                                 "QPushButton{border-image: url(:/trayicon/images/trans.png);}");
    connect(waggerybutton, SIGNAL(released()), this, SLOT(drawpixmap()));
    arrowbutton=new QRadioButton(tr(" "), this);
    arrowbutton->setStyleSheet("image: url(:/trayicon/images/arrow.png);");
    arrowbutton->move(-100,-100);
    penbutton=new QRadioButton(" ",this);
    penbutton->setStyleSheet("image: url(:/trayicon/images/blackpen.png);");
    penbutton->move(-100,-100);
    roundbutton=new QRadioButton(" ",this);
    roundbutton->setStyleSheet("image: url(:/trayicon/images/round.png);");
    roundbutton->move(-100,-100);
    //默认互斥
    this->setMouseTracking(true);//???
    isPressed = false;
    isRelease=0;
    ifpressed=false;  //new
    ifzoom=0;
    fun1=fun2=0;
    this->index = ind;

    if(ind==9)
    {
        index=0;
        issuicide=true;
    }
    intiaView();
    this->setPixmap(*pix_flog);
    setCursor(Qt::CrossCursor);
    this->show();
}

//初始化界面
void FullScreen::intiaView()
{
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect screenRect = desktopWidget->screenGeometry();
    g_nActScreenX = screenRect.width();
    g_nActScreenY = screenRect.height();
    this->resize(g_nActScreenX,g_nActScreenY);

    this->setFocusPolicy(Qt::StrongFocus);

    pixmap = new QPixmap;
    pixtemp= new QPixmap;
    *pixtemp= pixmap->grabWindow(QApplication::desktop()->winId());//图像编辑重新绘制时使用
    *pixmap = pixmap->grabWindow(QApplication::desktop()->winId());


    //设置雾层
    pix_flog = new QPixmap(*pixmap);
    fog = new QPixmap(g_nActScreenX,g_nActScreenY);
    //QPixmap fog(g_nActScreenX,g_nActScreenY);
    //背景白雾颜色及透明度

    if(issuicide==true)
    {
        fog->fill(QColor(28,116,166,100));
    }
    else if(index!=3)
    {
        if(index!=8)
            fog->fill(QColor(255,255,255,100));

        else
        {
            fog->fill(QColor(255,255,255,0));
            painter = new QPainter(pix_flog);
            QPixmap pixss;
            pixss.load(":/trayicon/images/bulescreen.png");
            painter->drawPixmap(0,0,g_nActScreenX,g_nActScreenY,pixss);
            cur->setPos(g_nActScreenX,g_nActScreenY);
            this->setPixmap(pixss);
            //小提示,蓝屏假象按两边
        }
    }
    else
        fog->fill(QColor(255,255,255,0));


    painter = new QPainter(pix_flog);
    painter->setFont(QFont("华文行楷",23));
    QPen pen(QColor(0,0,0));
    pen.setWidth(20);
    painter->drawPixmap(0,0,*fog);
    painter->setPen(QPen(QColor(0,0,0)));
}

void FullScreen::color1()
{
    isgreen=0;
    isred=0;
    isblue=0;
    penbutton->setStyleSheet("image: url(:/trayicon/images/blackpen.png);");
}
void FullScreen::color2()
{
    isred=255;
    isgreen=0;
    isblue=0;
    penbutton->setStyleSheet("image: url(:/trayicon/images/redpen.png);");
}
void FullScreen::color3()
{
    isblue=255;
    isred=0;
    isgreen=255;
    penbutton->setStyleSheet("image: url(:/trayicon/images/pencil.png);");
}

void FullScreen::thick1()
{
     penpix=1;
     tButton->setStyleSheet("border-image: url(:/trayicon/images/1px.png);");
}
void FullScreen::thick2()
{ penpix=3;
   tButton->setStyleSheet("border-image: url(:/trayicon/images/3px.png);");
}
void FullScreen::thick3()
{
    penpix=6;
    tButton->setStyleSheet("border-image: url(:/trayicon/images/6px.png);");
}



void FullScreen::changewinISO()
{if((height_y+width*sqrt(2))<g_nActScreenY-20)
    height=width*sqrt(2);
 else width=height*sqrt(2);





    if(index==0)setSelectedLabel();
    else if(index==2)setSelectedRound();
}

//矩形截图框架实现
void FullScreen::setSelectedLabel(){
    QPixmap pix = *pixmap;
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPixmap(0,0,*fog);

    if(!isRelease)
    {
        width = abs(endPoint.x() - origin.x());
        height = abs(endPoint.y() - origin.y());
        width_x = origin.x()  < endPoint.x() ? origin.x():endPoint.x();
        height_y = origin.y() < endPoint.y() ? origin.y():endPoint.y();
    }

    if(width!=0 && height != 0)
    {
        painter.drawPixmap(width_x,height_y,pixmap->copy(width_x,height_y,width,height));
        painter.setPen(QPen(QColor(255,0,0),1));
        painter.drawRect(QRect(width_x,height_y,width,height));
        painter.setBrush(QColor(255,0,0));

        QPoint temp(width_x+width+10,height_y+height);


        painter.setPen(QPen(QColor(0,0,0),2));
        if(isPressed||isRelease)
            painter.drawText(temp,tr("%1 x %2").arg(width).arg(height));

    }
    if(ifdrag&&isRelease)//拉完方框后执行
    {
        painter.drawEllipse(QPointF(width_x+width,height_y+height),4,4);
        painter.drawEllipse(QPointF(width_x,height_y+height),4,4);
        painter.drawEllipse(QPointF(width_x+width,height_y),4,4);
        painter.drawEllipse(QPointF(width_x,height_y),4,4);
        pButton->move(width_x+175,height+height_y);
        arrowbutton-> setGeometry(QRect(QPoint(width_x+10, height_y+height),
                                        QSize(51, 31)));
        roundbutton-> setGeometry(QRect(QPoint(width_x+111, height_y+height),
                                        QSize(61, 30)));
        penbutton->setGeometry(QRect(QPoint(width_x+62, height_y+height),
                                     QSize(51, 31)));
        editwinbutton->move(width_x-50, height_y+height+30);
        waggerybutton->setGeometry(QRect(QPoint(width_x+width+20, height_y+height+10),
                                         QSize(60, 60)));
        restbutton->setGeometry(QRect(QPoint(width_x-50, height_y+height+5),
                                      QSize(50, 20)));
        A4Button->setGeometry(QRect(QPoint(width_x-50, height_y+height-30),
                                      QSize(22, 33)));
        tButton->move(width_x+220,height+height_y+5);
         literalbutton->move(width_x+275,height+height_y+5);

        if (arrowbutton->isChecked())//箭头
        {
            QPainterPath arrowpath;
            QPointF arrowpointpeak=paintendPoint;
            QPointF arrowpointend=paintorigin;
            QPointF arrowvector=arrowpointpeak-arrowpointend;
            QPointF arrowmedium=arrowpointend+0.8*arrowvector-6*arrowvector/sqrt(arrowvector.x()*arrowvector.x()+arrowvector.y()*arrowvector.y());
            QPointF arrowperpendicular;

            arrowperpendicular.setX(arrowvector.y());
            arrowperpendicular.setY(-arrowvector.x());
            //int vertebrationdistance=arrowvector.manhattanLength();
            arrowpath.moveTo(arrowpointpeak);
            arrowpath.lineTo(arrowmedium+0.10*arrowperpendicular);
            arrowpath.lineTo(arrowmedium+0.03*arrowperpendicular);
            arrowpath.lineTo(arrowpointend);
            arrowpath.lineTo(arrowmedium-0.03*arrowperpendicular);
            arrowpath.lineTo(arrowmedium-0.10*arrowperpendicular);
            painter.fillPath (arrowpath, QBrush(QColor(isred,isgreen,isblue,150)));
            if(isPressed==false)//鼠标释放
            {
                QPainter painters(pixmap);
                painters.setRenderHint(QPainter::Antialiasing,true);
                painters.fillPath (arrowpath, QBrush(QColor(isred,isgreen,isblue,150)));

            }

        }

        if (roundbutton->isChecked())//箭头
        {bruchround.setColor(QColor(205,100,0,0));
         painter.setBrush(bruchround);

            painter.setPen(QPen(QColor(isred,isgreen,isblue),penpix));
            painter.drawRoundedRect(paintorigin.x(),paintorigin.y(),paintendPoint.x()-paintorigin.x(),paintendPoint.y()-paintorigin.y(),abs( paintendPoint.x()-paintorigin.x())/ 2
                                     ,abs( paintendPoint.y()-paintorigin.y())/ 2);
            if(isPressed==false)//鼠标释放
            {
                QPainter painters(pixmap);
                painters.setBrush(bruchround);
                painters.setRenderHint(QPainter::Antialiasing,true);
                painters.setPen(QPen(QColor(isred,isgreen,isblue),penpix));
                painters.drawRoundedRect(paintorigin.x(),paintorigin.y(),paintendPoint.x()-paintorigin.x(),paintendPoint.y()-paintorigin.y(),abs( paintendPoint.x()-paintorigin.x())/ 2,
                                         abs( paintendPoint.y()-paintorigin.y())/ 2);

            }

        }


    }

    this->setPixmap(pix);
}

void FullScreen::resetpixmap()
{   setCursor(Qt::CrossCursor);
    iswaggeryedit=false;
    *pixmap = *pixtemp;
    paintorigin=QPoint(0,0);
    paintendPoint=QPoint(0,0);
    if(index==0)setSelectedLabel();
    if(index==2)setSelectedRound();
}

void FullScreen::drawpixmap()
{   if(iswaggeryedit==true)iswaggeryedit=false;
    else iswaggeryedit=true;

    QCursor *myCursor=new QCursor(QPixmap(":/trayicon/images/30px.png"),-1,-1);

    if(iswaggeryedit==true) setCursor(*myCursor);
    else setCursor(Qt::CrossCursor);

    if(index==0)setSelectedLabel();
    if(index==2)setSelectedRound();

}


//椭圆截图框架实现
void FullScreen::setSelectedRound()
{    QPixmap pix = *pixmap;
     QPainter painter(&pix);
     painter.setRenderHint(QPainter::Antialiasing,true);
    if(!isRelease)
{
    width_x = origin.x()  < endPoint.x() ? origin.x():endPoint.x();
    height_y = origin.y() < endPoint.y() ? origin.y():endPoint.y();

    width = abs(endPoint.x() - origin.x());
    height = abs(endPoint.y() - origin.y());

}

    if(width > 5 &&height >5)
    {

        painter.setPen(QPen(QColor(120,0,205),1));
        painter.drawPixmap(0,0,*fog);

        QSize size(width,height);

        QPixmap mask(size);
        QPainter painter2(&mask);

        painter2.setRenderHint(QPainter::Antialiasing,true);
        painter2.setRenderHint(QPainter::SmoothPixmapTransform);
        if(!isobscure)
            painter2.fillRect(0,0,width,height,Qt::white);
        else painter2.fillRect(0,0,width,height,QColor(100,100,100,100));
        painter2.setBrush(QColor(0,0,0));
        painter2.drawRoundedRect(0,0,width,height,width / 2,height / 2);

        image = pixmap->copy(width_x,height_y,width,height);
        image.setMask(mask);

        painter.drawPixmap(width_x,height_y,image);
        painter.drawRoundedRect(width_x,height_y,width,height,width / 2,height / 2);

        QPoint temp(width_x+width+1,height_y+height-10);

        painter.setPen(QPen(QColor(0,0,0),2));
        painter.drawText(temp,tr("%1 x %2").arg(width).arg(height));


    }

    if(ifdrag&&isRelease)//拉完方框后执行
    {
        painter.setPen(QPen(QColor(120,0,205),1));
        painter.setBrush(QColor(120,0,205));
        painter.drawEllipse(QPointF(width_x+width,height_y+height),4,4);
        painter.drawEllipse(QPointF(width_x,height_y+height),4,4);
        painter.drawEllipse(QPointF(width_x+width,height_y),4,4);
        painter.drawEllipse(QPointF(width_x,height_y),4,4);
        pButton->move(width_x+175,height+height_y);
        arrowbutton-> setGeometry(QRect(QPoint(width_x+10, height_y+height),
                                        QSize(51, 31)));
        roundbutton-> setGeometry(QRect(QPoint(width_x+111, height_y+height),
                                        QSize(61, 30)));
        penbutton->setGeometry(QRect(QPoint(width_x+62, height_y+height),
                                     QSize(51, 31)));
        editwinbutton->move(width_x-50, height_y+height+30);
        waggerybutton->setGeometry(QRect(QPoint(width_x+width+20, height_y+height+10),
                                         QSize(60, 60)));
        restbutton->setGeometry(QRect(QPoint(width_x-50, height_y+height+5),
                                      QSize(50, 20)));
        A4Button->setGeometry(QRect(QPoint(width_x-50, height_y+height-30),
                                      QSize(22, 33)));
        tButton->move(width_x+220,height+height_y+6);
         literalbutton->move(width_x+275,height+height_y+6);

        if (arrowbutton->isChecked())//箭头
        {
            QPainterPath arrowpath;
            QPointF arrowpointpeak=paintendPoint;
            QPointF arrowpointend=paintorigin;
            QPointF arrowvector=arrowpointpeak-arrowpointend;
            QPointF arrowmedium=arrowpointend+0.8*arrowvector;
            QPointF arrowperpendicular;
            arrowperpendicular.setX(arrowvector.y());
            arrowperpendicular.setY(-arrowvector.x());
            //int vertebrationdistance=arrowvector.manhattanLength();
            arrowpath.moveTo(arrowpointpeak);
            arrowpath.lineTo(arrowmedium+0.10*arrowperpendicular);
            arrowpath.lineTo(arrowmedium+0.03*arrowperpendicular);
            arrowpath.lineTo(arrowpointend);
            arrowpath.lineTo(arrowmedium-0.03*arrowperpendicular);
            arrowpath.lineTo(arrowmedium-0.10*arrowperpendicular);
            painter.fillPath (arrowpath, QBrush(QColor(isred,isgreen,isblue,150)));
            if(isPressed==false)//鼠标释放
            {
                QPainter painters(pixmap);
                painters.setRenderHint(QPainter::Antialiasing,true);
                painter.fillPath (arrowpath, QBrush(QColor(isred,isgreen,isblue,150)));
            }

        }
        if (roundbutton->isChecked())//箭头
        {bruchround.setColor(QColor(205,100,0,0));
            painter.setBrush(bruchround);
            painter.setPen(QPen(QColor(isred,isgreen,isblue),penpix));
            painter.drawRoundedRect(paintorigin.x(),paintorigin.y(),paintendPoint.x()-paintorigin.x(),paintendPoint.y()-paintorigin.y(),abs( paintendPoint.x()-paintorigin.x())/ 2,abs( paintendPoint.y()-paintorigin.y())/ 2);

            if(isPressed==false)//鼠标释放
            {
                QPainter painters(pixmap);
                painters.setRenderHint(QPainter::Antialiasing,true);
                painters.setBrush(bruchround);

                painters.setPen(QPen(QColor(isred,isgreen,isblue),penpix));
                painters.drawRoundedRect(paintorigin.x(),paintorigin.y(),paintendPoint.x()-paintorigin.x(),paintendPoint.y()-paintorigin.y(),abs( paintendPoint.x()-paintorigin.x())/ 2,abs( paintendPoint.y()-paintorigin.y())/ 2);


            }

        }



    }
this->setPixmap(pix);
}

//多边形及任意截图框架实现
void FullScreen::setSelectedArbitrary()
{
    QPixmap pix = *pixmap;
    QPainter painter(&pix);
    painter.setPen(QPen(QColor(205,20,0),1));

    //painterz.drawLine(Polygonpoints[Polygonpoints.size()-2],
    //Polygonpoints[Polygonpoints.size()-1]);
    //如果会卡就不进行重新绘图
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawPath(path);
    painter.fillPath (path, QBrush(QColor(205,100,0,50)));
    painter.setPen(QPen(QColor(155,0,255),2));
    painter.drawEllipse(originP, 6, 6);
    painter.drawEllipse(originP, 3, 3);
    painter.setPen(QPen(QColor(205,20,0),1));
    for(int j=0;j<Roundfocuspoints.size();j++)
    {
        QBrush bruch(Qt::SolidPattern);
        bruch.setColor(QColor(205,100,0,50));
        painter.setBrush(bruch);
        painter.drawEllipse(Roundfocuspoints[j],Roundradius[j],Roundradius[j]);
    }


    if(IsArbitraryEnd==true)
    {   if(Polygonpoints.size()>1)
        {
            for(int i=0;i<Polygonpoints.size();i++)
            {
                if(originP.x()>Polygonpoints[i].x())
                    originP.setX(Polygonpoints[i].x());

                if(originP.y()>Polygonpoints[i].y())
                    originP.setY(Polygonpoints[i].y());

            }
            for(int j=0;j<Polygonpoints.size();j++)
            {
                if(endP.x()<Polygonpoints[j].x())
                    endP.setX(Polygonpoints[j].x());

                if(endP.y()<Polygonpoints[j].y())
                    endP.setY(Polygonpoints[j].y());
            }
        }
        else if(Roundfocuspoints.size()>0)
        {
            //如果没有左键的点,初始化右键椭圆的点
            originP.rx()=Roundfocuspoints[0].x()-Roundradius[0];
            originP.ry()=Roundfocuspoints[0].y()-Roundradius[0];
            endP.rx()=Roundfocuspoints[0].x()+Roundradius[0];
            endP.ry()=Roundfocuspoints[0].y()+Roundradius[0];
        }

        if(Roundfocuspoints.size()>0)
        {
            int i,j;
            for(i=0;i<Roundfocuspoints.size();i++)
            {
                if((Roundfocuspoints[i].x()-Roundradius[i])<=0)
                {
                    originP.setX(0);break;
                }
                //X超过范围直接设置为0
                else
                {

                    if(originP.x()>Roundfocuspoints[i].x()-Roundradius[i])
                        originP.setX(Roundfocuspoints[i].x()-Roundradius[i]);


                }

                if((Roundfocuspoints[i].y()-Roundradius[i])<=0)
                {
                    originP.setY(0);
                    break;
                }//Y超过范围直接设置为0
                else
                {
                    if(originP.y()>Roundfocuspoints[i].y()-Roundradius[i])
                        originP.setY(Roundfocuspoints[i].y()-Roundradius[i]);
                }
            }
            for(j=0;j<Roundfocuspoints.size();j++)
            {
                if((Roundfocuspoints[j].y()+Roundradius[j])>=g_nActScreenY)
                {
                    endP.setY(g_nActScreenY);
                    break;
                    //超过范围直接设置为最大
                }
                else
                {

                    if(endP.y()<Roundfocuspoints[j].y()+Roundradius[j])
                        endP.setY(Roundfocuspoints[j].y()+Roundradius[j]);
                }
                if((Roundfocuspoints[j].x()+Roundradius[j])>=g_nActScreenX)
                {
                    endP.setX(g_nActScreenX);
                    break;

                    //超过范围直接设置为最大
                }
                else

                {
                    if(endP.x()<Roundfocuspoints[j].x()+Roundradius[j])
                        endP.setX(Roundfocuspoints[j].x()+Roundradius[j]);


                }
            }
        }

        //两个if找出边界

        width =endP.x() - originP.x();
        height =endP.y() - originP.y();
        width_x=originP.x();
        height_y=originP.y();

        image = pixmap->copy(width_x,height_y,width,height);

        QPixmap mask(width,height);


        QPainter painter2(&mask);
        painter2.setRenderHint(QPainter::Antialiasing,true);
        if(isobscure==0)
            painter2.fillRect(0,0,width,height,Qt::white);
        else    painter2.fillRect(0,0,width,height,QColor(100,100,100,100));

        painter2.setBrush(QColor(0,0,0));

        if(Polygonpoints.size()>0)
        {
            QPainterPath pathz;
            pathz.moveTo(Polygonpoints[0]-originP);

            for(int i=1;i<Polygonpoints.size();i++)
            {
                pathz.lineTo(Polygonpoints[i]-originP);
            }
            painter2.setRenderHint(QPainter::Antialiasing,true);
            painter2.drawPath(pathz);
        }
        if(Roundfocuspoints.size()>0)
        {
            for(int j=0;j<Roundfocuspoints.size();j++)
            {
                painter2.drawEllipse(Roundfocuspoints[j]-originP,Roundradius[j],Roundradius[j]);
            }
        }
        image.setMask(mask);
    }

    this->setPixmap(pix);
}


//鼠标按下
void FullScreen::mousePressEvent(QMouseEvent *event)
{
    ifzoom=1;
    isPressed=true;
    ifpressed=true;//new

    //上面那几个变量真特么恶心
    if(Qt::LeftButton == event->button())
    {
        if(ifdrag&&isRelease&&!(penbutton->isChecked())
               &&!(arrowbutton->isChecked())
               &&(iswaggeryedit==false)
                &&!(roundbutton->isChecked())
                &&!isliteraledit
               )//表示不正在图像编辑中
      {
            origin2=event->pos();
            if(abs(event->pos().x()-width_x)<5&&abs(event->pos().y()-height_y)<5){iseditlefttop=true;setCursor(Qt::SizeAllCursor);}
            else if(abs(event->pos().x()-width_x-width)<5&&abs(event->pos().y()-height_y-height)<5){iseditrightbottom=true;setCursor(Qt::SizeAllCursor);}
            else if(abs(event->pos().x()-width_x-width)<5&&abs(event->pos().y()-height_y)<5){iseditrighttop=true;setCursor(Qt::SizeAllCursor);}
            else if(abs(event->pos().x()-width_x)<5&&abs(event->pos().y()-height_y-height)<5){iseditleftbottom=true;setCursor(Qt::SizeAllCursor);}
            else if((event->pos().x()>width_x+2)&&(event->pos().x()<width+width_x-2)&&(event->pos().y()<height+height_y-2)&&(event->pos().y()>height_y+2)){iseditall=true;setCursor(Qt::SizeAllCursor);}

            widthtemp=width;
            heighttemp=height;
            width_xtemp=width_x;
            height_ytemp=height_y;
      }





        if(index!=3)
        {
            if(isRelease==0)
            {
                origin = event->pos();
            }
        }
        else
        {

            if(Isfirstpress == true)
            {
                path.moveTo(event->pos());
                originP= event->pos();
                endP= event->pos();
                Polygonpoints<<event->pos();
                setSelectedArbitrary();
                Isfirstpress =false;
            }
            else
            {
                endP=event->pos();
                Polygonpoints<<event->pos();
                path.lineTo(event->pos());
                if((abs(Polygonpoints[Polygonpoints.size()-1].x()-originP.x())<=6)
                        &&(abs(Polygonpoints[Polygonpoints.size()-1].y()-originP.y())<=6))
                {
                    IsArbitraryEnd=true;
                    setSelectedArbitrary();
                    this->close();
                    function();

                }
                setSelectedArbitrary();
            }


        }
        //以上是针对index的


        if(ifdrag&&isRelease)
        {
            paintorigin=event->pos();
        }
        //绘图区,滑稽鼠标形状改变,所以专享
        if(iswaggeryedit==true)
        {
            QPainter wapainter(pixmap);
            wapainter.setRenderHint(QPainter::Antialiasing, true);
            wapainter.drawPixmap(QRect(event->pos().x()-15,event->pos().y()-15,30,30), QPixmap(":/trayicon/images/30px.png"));
        }
        if(penbutton->isChecked())
        {
            path2.moveTo(event->pos());
        }
        if(isliteraledit)
        {

            texttype->setGeometry(QRect(event->pos(),
                                                  QSize(200,16+penpix*4)));
            texttype->setFont(QFont("宋体" , 8+penpix*2 ,  QFont::Bold));
            texttype->setFocus(Qt::OtherFocusReason);




        }

    }

    if(Qt::RightButton==event->button())
    {
        if(index!=3){this->close();

        }
        else{
            isrightbuttonpressed=true;
            endP=event->pos();
            Roundfocuspoints<<event->pos();
            time.start();
        }
        //Roundradius<<animation_n;
        //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        //animation_n=qrand()%40+2;
    }

}
//鼠标移动
void FullScreen::mouseMoveEvent(QMouseEvent *event)
{
    setMouseTracking(true);

    if(index!=3)
    {

        if(isRelease==0 && isPressed)
        {
            endPoint = event->pos();
            //矩形截图
            if(index == 0)
            {
                setSelectedLabel();
            }
            //动态获取裁剪图片

            //圆形截图
            if(index == 2)
            {
                setSelectedRound();
            }
        }
        //在按下之前的移动时的放大镜
        if(isRelease==0&&!isPressed)
        {
            ifzoom=1;
            endPoint = event->pos();

            origin.setX(endPoint.x()-1);
            origin.setY(endPoint.y()-1);

            setSelectedLabel();


        }

        if(isRelease==1&&ifdrag&&isPressed)//现在是编辑模式下拉完图的时候,并按下鼠标
        {

            paintendPoint=event->pos();


            //
            if(iseditlefttop)
            {
                width_x=width_xtemp+paintendPoint.x()-origin2.x();
                height_y=height_ytemp+paintendPoint.y()-origin2.y();
                width=abs(widthtemp-paintendPoint.x()+origin2.x());
                height=abs(heighttemp-paintendPoint.y()+origin2.y());

            }
            else if(iseditrighttop)
            {
                height_y=height_ytemp+paintendPoint.y()-origin2.y();
                width=abs(widthtemp+paintendPoint.x()-origin2.x());
                height=abs(heighttemp-paintendPoint.y()+origin2.y());

            }
            else if(iseditleftbottom)
            {
                width_x=width_xtemp+paintendPoint.x()-origin2.x();
                width=abs(widthtemp-paintendPoint.x()+origin2.x());
                height=abs(heighttemp+paintendPoint.y()-origin2.y());

            }
            else if(iseditrightbottom)
            {
                width=abs(widthtemp+paintendPoint.x()-origin2.x());
                height=abs(heighttemp+paintendPoint.y()-origin2.y());

            }
            else if(iseditall)
            {

                width_x=width_xtemp+paintendPoint.x()-origin2.x();
                height_y=height_ytemp+paintendPoint.y()-origin2.y();

            }

            if(index==0)setSelectedLabel();
            else if(index==2)setSelectedRound();
        }

    }
    else if(isrightbuttonpressed==false)//(index==3),isrightButtonpressed==false是为了防止右键崩溃
    {
        endP= event->pos();
        ifzoom=1;
        setSelectedArbitrary();

        if(abs(originP.x()-(event->pos().x()))<7&& abs(originP.y()-(event->pos().y()))<7
            &&Polygonpoints.size()>10
                &&(abs(originP.x()-(event->pos().x()))>4
                ||abs(originP.y()-(event->pos().y()))>4))
        {
            if(Polygonpoints.size()>10)
            {
                cur->setPos(originP.x(),originP.y());
            }

        }

        if(isPressed)
        {
            //吸附,先吸附再入点

            Polygonpoints<<event->pos();
            path.lineTo(event->pos());
            setSelectedArbitrary();
        }

    }

    //按功能分,不需要index判断
    //if(ifdrag&&isRelease)
    //{
        //paintendPoint=event->pos();
    //}
    //开始更新绘图坐标paintendpoint
    //没按下就暂时不考虑

    if(isPressed)
    {
        if(iswaggeryedit==true)
        {
            QPainter wapainter(pixmap);
            wapainter.setRenderHint(QPainter::Antialiasing, true);
            wapainter.drawPixmap(QRect(event->pos().x()-15,event->pos().y()-15,30,30), QPixmap(":/trayicon/images/30px.png"));
            if(index==0)setSelectedLabel();
            else if(index==2)setSelectedRound();
        }
        //还是觉得这里写最好
        if(penbutton->isChecked())
        {
            path2.lineTo(event->pos());
            QPainter painter(pixmap);

              painter.setPen(QPen(QColor(isred,isgreen,isblue),penpix));

            painter.drawPath(path2);
        }
    }
}

//鼠标释放
void FullScreen::mouseReleaseEvent(QMouseEvent *event)
{ Q_UNUSED( event )
            isPressed = false;isRelease=1;
    if(index!=3)
    {
        ifzoom=0;
        if(!ifdrag)
        {
            this->close();
            function();
        }
    }
    if(index==3&&Qt::RightButton==event->button())
    {
        isrightbuttonpressed=false;
        int time_Diff = time.elapsed();
        float f = time_Diff/1000.0;//单位秒
        Roundradius<<2+50*f+105*f*f+25*f*f*f;
        setSelectedArbitrary();
    }


    if(ifdrag)
    {

        paintendPoint=event->pos();

        if(iseditlefttop)
        {
            iseditlefttop=false;
            setCursor(Qt::CrossCursor);
        }
        else if(iseditrighttop)
        {
            iseditrighttop=false;
            setCursor(Qt::CrossCursor);
        }
        else if(iseditleftbottom)
        {
            iseditleftbottom=false;
            setCursor(Qt::CrossCursor);
        }
        else if(iseditrightbottom)
        {
            iseditrightbottom=false;
            setCursor(Qt::CrossCursor);
        }
        else if(iseditall)
        {
            iseditall=false;
            setCursor(Qt::CrossCursor);
        }


        if(index==0)setSelectedLabel();
        else if(index==2)setSelectedRound();
    }

    if(penbutton->isChecked())
    {
        path2 = QPainterPath() ;
    }


}

//鼠标双击
void FullScreen::mouseDoubleClickEvent(QMouseEvent *ev)
{ Q_UNUSED( ev )
            if(!ifdrag)//需要进行图像编辑时双击无效
    {
        if(index!=3)
        {
            this->close();
            function();

        }

        if(index==3)
        {


            IsArbitraryEnd=true;
            setSelectedArbitrary();
            this->close();
            function();



        }

    }
}



//键盘按下
void FullScreen::keyPressEvent(QKeyEvent *e)
{

    //退出
    if(e->key() == Qt::Key_Escape)
    {this->close();
    }




    //实现功能
    if(e->key()==Qt::Key_Return)
    {
        if(index!=3)
        {
            if(!isliteraledit){this->close();function();}
            else {drawliteral();}
        }
        if(index==3)
        {
            IsArbitraryEnd=true;
            setSelectedArbitrary();
            this->close();
            function();
        }


    }



    //键盘微调
    if(isRelease==1&&!isliteraledit)
    {
        if(e->key()== Qt::Key_A)
        {
            width_x-=1;
        }
        if(e->key()==Qt::Key_D)
        {
           width_x+=1;
        }
        if(e->key()==Qt::Key_W)
        {
           height_y-=1;
        }
        if(e->key()==Qt::Key_S)
        {
            height_y+=1;
        }
    }

    //矩形截图
    if(index == 0)
    {
        setSelectedLabel();
    }


    //圆形截图
    if(index == 2)
    {
        setSelectedRound();
    }

}


//功能实现
void FullScreen::function()
{
    setCursor(Qt::ArrowCursor);
    //矩形截图
    if(index == 0 )
    {

        QPixmap pix = pixmap->copy(width_x,height_y,width,height);
        shot=pix;
        if(fun2==1)
        {
            //文件路径示例   /home/Qt_picture/new.png
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("MM.dd-hh.mm.ss");

            // 保存文件全路径
            str = dirpath + "/" + str+ ".png";
            // 保存
            pix.save(str, "png");


        }
        //连接到图片程序
        if(fun2==3)
        {

            //文件路径示例   /home/Qt_picture/new.png
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("MM.dd-hh.mm.ss");

            // 保存文件全路径
            str = dirpath + "/" + str+ ".png";

            // 保存
            pix.save(str, "png");
            QString picPath = "file:///" + str;
            QDesktopServices::openUrl(QUrl(picPath));
        }

        //把文件放到剪切板上
        if(fun1==2)
            QApplication::clipboard()->setImage(pix.toImage());


    }
    //圆形截图
    if(index == 2||index==3)
    {

        shot=image;
        //直接保存到文件管理
        if(fun2==1)
        {
            //文件路径示例   /home/Qt_picture/new.png
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("MM.dd-hh.mm.ss");

            // 保存文件全路径
            str = dirpath + "/" + str+ ".png";
            // 保存
            image.save(str, "png");

        }
        //连接到图片程序
        if(fun2==3)
        {


            //文件路径示例   /home/Qt_picture/new.png
            QDateTime time = QDateTime::currentDateTime();
            QString str = time.toString("MM.dd-hh.mm.ss");

            // 保存文件全路径
            str = dirpath + "/" + str+ ".png";
            // 保存
            image.save(str, "png");
            //获取当前路径
            QString picPath = "file:///" + str;
            QDesktopServices::openUrl(QUrl(picPath));
        }



        //把文件放到剪切板上
        if(fun1==2)
            QApplication::clipboard()->setImage(image.toImage());


    }

}



//放大镜实现
void FullScreen::paintEvent(QPaintEvent *event){

    QLabel::paintEvent(event);
    if(index==8)
    {
        QPainter painter(this);
        QPixmap pixxx;
        pixxx.load(":/trayicon/images/bulescreen.png");
        painter.drawPixmap(0,0,g_nActScreenX,g_nActScreenY,pixxx);
    }
    if(ifzoom&&index == 0&&!isRelease)
    {
        //放大框
        QRect zoomRect=QRect(endPoint.x()-10,endPoint.y()-10,20,20);

        QPixmap Map=pixmap->copy(zoomRect);
        QPainter zoompainter(this);
        QPen pen;
        QBrush brush(QColor(0, 0, 0));
        QFont font("Microsoft YaHei",8);

        int x=endPoint.x();
        int y=endPoint.y();
        if(y+140>=g_nActScreenY)
        {
            y=0;
        }
        if(x+110>=g_nActScreenX)
        {
            x=0;
        }
        //显示大小->显示坐标
        int width = endPoint.x();
        int height =endPoint.y();
        //显示放大点的RGB
        int red,green,blue;

        red = qRed(pixmap->toImage().pixel(endPoint.x(),endPoint.y()));
        green = qGreen(pixmap->toImage().pixel(endPoint.x(),endPoint.y()));
        blue = qBlue(pixmap->toImage().pixel(endPoint.x(),endPoint.y()));

        zoompainter.drawPixmap(x+10, y+10, 100, 100, Map);
        //红十字
        pen.setColor(QColor(255,0, 0));
        zoompainter.setPen(pen);
        zoompainter.drawLine(QPointF(0, endPoint.y()), QPointF(g_nActScreenX, endPoint.y()));
        zoompainter.drawLine(QPointF(endPoint.x(),0 ), QPointF(endPoint.x(), g_nActScreenY));

        pen.setColor(QColor(0, 255, 0, 125));
        pen.setWidthF(3);
        zoompainter.setPen(pen);
        zoompainter.drawLine(x+10+50, y+15, x+10+50, y+10+95);
        zoompainter.drawLine(x+15, y+10+50, x+10+95, y+10+50);
        pen.setColor(QColor(0,0,0));
        pen.setWidth(1);
        zoompainter.setPen(pen);
        zoompainter.drawRect(x+10, y+10, 100, 100);
        //信息框
        brush.setColor(QColor(0,0,0,200));
        zoompainter.setBrush(brush);
        zoompainter.drawRect(x+10, y+10+100,100,30);
        QRect infoRect_1 = QRect(x+10,y+10+100,100,15);
        QRect infoRect_2 = QRect(x+10,y+10+100+15,100,15);
        pen.setColor(QColor(255,255,255));
        zoompainter.setPen(pen);
        zoompainter.setFont(font);
        if(isPressed)
        {
            zoompainter.drawText(infoRect_1,Qt::AlignHCenter|Qt::AlignVCenter,QString("( %1 , %2 )").arg(width).arg(height));
        }
        zoompainter.drawText(infoRect_2,Qt::AlignHCenter|Qt::AlignVCenter,QString("RGB(%1,%2,%3)").arg(red).arg(green).arg(blue));
    }
    if(ifzoom&&index == 3)
    {
        //放大框
        QRect zoomRect=QRect(endP.x()-10,endP.y()-10,20,20);

        QPixmap Map=pixmap->copy(zoomRect);
        QPainter zoompainter(this);
        QPen pen;
        QBrush brush(QColor(0, 0, 0));
        QFont font("Microsoft YaHei",8);
        int red,green,blue;    //显示放大点的RGB

        red = qRed(pixmap->toImage().pixel(endP.x(),endP.y()));
        green = qGreen(pixmap->toImage().pixel(endP.x(),endP.y()));
        blue = qBlue(pixmap->toImage().pixel(endP.x(),endP.y()));
        int x=endP.x();int y=endP.y();
        if(y+140>=g_nActScreenY)
        {
            y=y-200;
        }
        if(x+110>=g_nActScreenX)
        {
            x=x-200;
        }
        if(y-140<=0)
        {
            y=y+100;
        }
        if(x-110<=0)
        {
            x=x+100;
        }

        zoompainter.drawPixmap(x+10,y+10, 100, 100, Map);

        //红十字
        pen.setColor(QColor(255,0, 0));
        zoompainter.setPen(pen);
        zoompainter.drawLine(QPointF(0, endP.y()), QPointF(g_nActScreenX, endP.y()));
        zoompainter.drawLine(QPointF(endP.x(),0 ), QPointF(endP.x(), g_nActScreenY));


        pen.setColor(QColor(0, 255, 0, 125));
        pen.setWidthF(3);
        zoompainter.setPen(pen);
        zoompainter.drawLine(x+10+50, y+15, x+10+50, y+10+95);
        zoompainter.drawLine(x+15, y+10+50, x+10+95, y+10+50);
        pen.setColor(QColor(0,0,0));
        pen.setWidth(1);
        zoompainter.setPen(pen);
        zoompainter.drawRect(x+10, y+10, 100, 100);
        //信息框
        brush.setColor(QColor(0,0,0,200));
        zoompainter.setBrush(brush);
        zoompainter.drawRect(x+10, y+10+100,100,30);
        QRect infoRect_1 = QRect(x+10,y+10+100,100,15);
        QRect infoRect_2 = QRect(x+10,y+10+100+15,100,15);
        pen.setColor(QColor(255,255,255));
        zoompainter.setPen(pen);
        zoompainter.setFont(font);
        zoompainter.drawText(infoRect_1,Qt::AlignHCenter|Qt::AlignVCenter,QString("( %1 , %2 )").arg(endP.x()).arg(endP.y()));
        zoompainter.drawText(infoRect_2,Qt::AlignHCenter|Qt::AlignVCenter,QString("RGB(%1,%2,%3)").arg(red).arg(green).arg(blue));
    }


}

FullScreen::~FullScreen()
{


}


//功能修改
void FullScreen::refun(int f1,int f2,bool dr,QString p,bool mini,int isblur)
{
    minimizeFulls=mini;
    fun1=f1;
    fun2=f2;
    ifdrag=dr;
    dirpath=p;
    isobscure=isblur;
}

void FullScreen::on_literalButton_clicked()
{ if(isliteraledit==true){isliteraledit=false;setCursor(Qt::CrossCursor);texttype->move(-100,-100);}
        else {isliteraledit=true;setCursor(Qt::IBeamCursor);}


        if(index==0)setSelectedLabel();
        if(index==2)setSelectedRound();


}
void FullScreen::drawliteral()
{   literal=texttype->text();

    QPainter painters(pixmap);
    painters.setPen(QColor(isred,isgreen,isblue));
    QFont font("宋体", 8+penpix*2,QFont::Bold);
    painters.setFont(font);
    painters.drawText(paintorigin+QPointF(3,8+penpix*3+4),literal);
    texttype->clear();
}
