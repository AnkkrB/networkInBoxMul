#-------------------------------------------------
#
# Project created by QtCreator 2011-01-30T17:34:09
#
#-------------------------------------------------

QT       += core gui network
QT       +=widgets

CONFIG += static


win32:LIBS += -LD:\AirPcap_Devpack_4_1_1_1838\WinPcap_Devpack\Lib -lwpcap
win32:INCLUDEPATH += D:\AirPcap_Devpack_4_1_1_1838\WinPcap_Devpack\Include

win32:LIBS += -LD:\AirPcap_Devpack_4_1_1_1838\Airpcap_Devpack\lib -lwpcap
win32:INCLUDEPATH += D:\AirPcap_Devpack_4_1_1_1838\Airpcap_Devpack\include

TARGET = QtNetEmu
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    networkinterface.cpp \
    reader_thread.cpp \
    buffer.cpp \
    writer_thread.cpp \
    PerformanceTimers.cpp \
    monitorthread.cpp

HEADERS  += mainwindow.h \
    networkinterface.h \
    reader_thread.h \
    buffer.h \
    writer_thread.h \
    PerformanceTimers.h \
    monitorthread.h \
    CollectedStatistics.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    test.txt \
    buffer.txt
