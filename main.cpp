#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    Widget w;

    int g_nActScreenX;   //屏幕尺寸
    int g_nActScreenY;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect screenRect = desktopWidget->screenGeometry();
    g_nActScreenX = screenRect.width();
    g_nActScreenY = screenRect.height();
    w.setGeometry(0.5*(screenRect.width())-367,0.1*(screenRect.height()),734,345);
    w.show();

    return a.exec();
}
