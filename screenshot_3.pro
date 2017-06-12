QT       += core gui
QT += widgets
QT += multimedia multimediawidgets

include(qxtglobalshortcut/qxtglobalshortcut.pri)
TARGET = screenshot_3
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS


RC_ICONS=fileico.ico

SOURCES += main.cpp\
        widget.cpp \
    fullscreen.cpp

HEADERS  += widget.h \
    fullscreen.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc

DISTFILES += \
    images/保存 .png \
    images/背景.png \
    images/叉.png \
    images/多边形 .png \
    images/矩形 .png \
    images/全屏   .png \
    images/设置  .png \
    images/缩小.png \
    images/圆形 .png
