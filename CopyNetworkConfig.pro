#-------------------------------------------------
#
# Project created by QtCreator 2017-02-25T17:01:07
#
#-------------------------------------------------

QT += core gui
QT += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CopyNetworkConfig
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    helpdialog.cpp \
    versiondialog.cpp

HEADERS  += mainwindow.h \
    tablemodel.h \
    helpdialog.h \
    versiondialog.h

FORMS    += mainwindow.ui \
    helpdialog.ui \
    versiondialog.ui

RESOURCES += \
    icon.qrc
