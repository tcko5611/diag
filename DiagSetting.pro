#-------------------------------------------------
#
# Project created by QtCreator 2019-09-18T18:18:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DiagSetting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DiagResultDialog.cpp \
    DiagSetting.cpp \
    DiagSettingDialog.cpp \
    JsonUtils.cpp \
    QAfData.cpp

HEADERS  += mainwindow.h \
    DiagResultDialog.h \
    DiagSetting.h \
    DiagSettingDialog.h \
    JsonUtils.h \
    QAfData.h

FORMS    += mainwindow.ui \
    DiagResultDialog.ui \
    DiagSettingDialog.ui
