#-------------------------------------------------
#
# Project created by QtCreator 2019-05-20T21:34:59
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Repository_Management_System
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    testdialog.cpp \
    buyer_newpurchase.cpp \
    admin_check.cpp \
    dbhelper.cpp \
    admin_detailandcheck.cpp \
    buyer_checklacklog.cpp \
    admin_main.cpp \
    admin_goodsmanagement.cpp \
    admin_goodsedit.cpp \
    buyer_newpurchaselog.cpp \
    buyer_main.cpp \
    seller_main.cpp \
    seller_newshipment.cpp \
    seller_newshipmentlog.cpp \
    buyer_mypurchase.cpp

HEADERS  += mainwindow.h \
    testdialog.h \
    buyer_newpurchase.h \
    admin_check.h \
    dbhelper.h \
    admin_detailandcheck.h \
    buyer_checklacklog.h \
    admin_main.h \
    admin_goodsmanagement.h \
    admin_goodsedit.h \
    buyer_newpurchaselog.h \
    buyer_main.h \
    seller_main.h \
    seller_newshipment.h \
    seller_newshipmentlog.h \
    buyer_mypurchase.h

FORMS    += mainwindow.ui \
    testdialog.ui \
    buyer_newpurchase.ui \
    admin_check.ui \
    admin_detailandcheck.ui \
    buyer_checklacklog.ui \
    admin_main.ui \
    admin_goodsmanagement.ui \
    admin_goodsedit.ui \
    buyer_newpurchaselog.ui \
    buyer_main.ui \
    seller_main.ui \
    seller_newshipment.ui \
    seller_newshipmentlog.ui \
    buyer_mypurchase.ui

RESOURCES += \
    resource.qrc
