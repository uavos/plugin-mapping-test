#-------------------------------------------------
#
# Project created by QtCreator 2017-06-27T22:19:09
#
#-------------------------------------------------

QT      -= gui
QT      += qml quick quickwidgets  positioning
CONFIG  += qt exception c++11

TARGET = PhotoPlan/$$qtLibraryTarget(photoplan)
include( ../../gcu_plugin.pri )

DEFINES += PHOTOPLAN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        photoplan_plugin.cpp \
    photoplanmodel.cpp

HEADERS += \
        photoplan_plugin.h \
    photoplanmodel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    photoplan.qrc
