#-------------------------------------------------
#
# Project created by QtCreator 2025-07-08T19:24:45
#
#-------------------------------------------------

QT       += core gui sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartCommunity
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        login.cpp \
    database.cpp \
    managermainwindow.cpp \
    staffmainwindow.cpp \
    usermainwindow.cpp \
    leaveapprovewidget.cpp \
    attendancewidget.cpp \
    staffmanagewidget.cpp \
    sysinitializewidget.cpp \
    monthlyattendancewidget.cpp \
    leavewidget.cpp \
    attendancerecordwidget.cpp \
    ownerrecordwidget.cpp \
    ownerinfoeditwidget.cpp \
    ownerinfoquerywidget.cpp \
    parkingmanagewidget.cpp \
    parkingrentwidget.cpp \
    pricemaintenancewidget.cpp \
    offlinepaywidget.cpp \
    paymentquerywidget.cpp \
    warrantyquerywidget.cpp \
    repairprocesswidget.cpp \
    faultreportwidget.cpp \
    repairquerywidget.cpp \
    repaircommentwidget.cpp \
    addstaffwidget.cpp \
    houseregistrationwidget.cpp \
    addannouncementwidget.cpp \
    carrecordwidget.cpp \
    parkingapplywidget.cpp \
    myparkingspotwidget.cpp \
    caridentificationwidget.cpp \
    selfpaymentwidget.cpp \
    userpaymentwidget.cpp

HEADERS += \
        login.h \
    database.h \
    managermainwindow.h \
    staffmainwindow.h \
    usermainwindow.h \
    leaveapprovewidget.h \
    attendancewidget.h \
    staffmanagewidget.h \
    sysinitializewidget.h \
    monthlyattendancewidget.h \
    leavewidget.h \
    attendancerecordwidget.h \
    ownerrecordwidget.h \
    ownerinfoeditwidget.h \
    ownerinfoquerywidget.h \
    parkingmanagewidget.h \
    parkingrentwidget.h \
    pricemaintenancewidget.h \
    offlinepaywidget.h \
    paymentquerywidget.h \
    warrantyquerywidget.h \
    repairprocesswidget.h \
    faultreportwidget.h \
    repairquerywidget.h \
    repaircommentwidget.h \
    addstaffwidget.h \
    houseregistrationwidget.h \
    addannouncementwidget.h \
    carrecordwidget.h \
    parkingapplywidget.h \
    myparkingspotwidget.h \
    caridentificationwidget.h \
    selfpaymentwidget.h \
    userpaymentwidget.h

FORMS += \
        login.ui \
    managermainwindow.ui \
    staffmainwindow.ui \
    usermainwindow.ui \
    leaveapprovewidget.ui \
    attendancewidget.ui \
    staffmanagewidget.ui \
    sysinitializewidget.ui \
    monthlyattendancewidget.ui \
    leavewidget.ui \
    attendancerecordwidget.ui \
    ownerrecordwidget.ui \
    ownerinfoeditwidget.ui \
    ownerinfoquerywidget.ui \
    parkingmanagewidget.ui \
    parkingrentwidget.ui \
    pricemaintenancewidget.ui \
    offlinepaywidget.ui \
    paymentquerywidget.ui \
    warrantyquerywidget.ui \
    repairprocesswidget.ui \
    faultreportwidget.ui \
    repairquerywidget.ui \
    repaircommentwidget.ui \
    addstaffwidget.ui \
    houseregistrationwidget.ui \
    addannouncementwidget.ui \
    carrecordwidget.ui \
    parkingapplywidget.ui \
    myparkingspotwidget.ui \
    caridentificationwidget.ui \
    selfpaymentwidget.ui \
    userpaymentwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    qrc.qrc
