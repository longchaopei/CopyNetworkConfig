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
    versiondialog.cpp \
    coverinfodialog.cpp \
    exceloperate.cpp \
    vpndialog.cpp \
    waitdialog.cpp

HEADERS  += mainwindow.h \
    tablemodel.h \
    helpdialog.h \
    versiondialog.h \
    datatype.h \
    utils/Log.h \
    coverinfodialog.h \
    exceloperate.h \
    vpndialog.h \
    waitdialog.h

FORMS    += mainwindow.ui \
    helpdialog.ui \
    versiondialog.ui \
    coverinfodialog.ui \
    vpndialog.ui \
    waitdialog.ui

RESOURCES += \
    icon.qrc
