#-------------------------------------------------
#
# Project created by QtCreator 2017-10-04T16:05:53
#
#-------------------------------------------------

QT       += core gui
QT       += core
QT       += network
QT += svg
//CONFIG   += c++11 console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QUdpSocket
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


SOURCES += \
        main.cpp \
        UI/mainwindow.cpp \
    Util/udpclient.cpp \
    UI/authwindow.cpp \
    Util/distance_damerau_levenshtein.cpp \
    Util/floodtimer.cpp \
    UI/Widgets/authlineedit.cpp \
    UI/Widgets/clickablelabel.cpp \
    UI/Widgets/globaltextedit.cpp \
    UI/Widgets/privatetextedit.cpp \
    UI/Widgets/wraplabel.cpp \
    UI/Widgets/affiximagewidget.cpp \
    UI/Widgets/sendwidget.cpp \
    UI/Widgets/imageview.cpp

HEADERS += \
        UI/mainwindow.h \
    Util/udpclient.h \
    Config/def.h \
    UI/authwindow.h \
    Util/distance_damerau_levenshtein.h \
    UI/Widgets/clickablelabel.h \
    Util/floodtimer.h \
    UI/Widgets/authlineedit.h \
    UI/Widgets/globaltextedit.h \
    UI/Widgets/privatetextedit.h \
    UI/Widgets/wraplabel.h \
    UI/Widgets/affiximagewidget.h \
    UI/Widgets/sendwidget.h \
    UI/Widgets/imageview.h

FORMS += \
        UI/mainwindow.ui \
    UI/authwindow.ui

RESOURCES += \
    Resources/resources.qrc
