#-------------------------------------------------
#
# Project created by QtCreator 2016-07-26T14:24:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = osgViewerQt5
TEMPLATE = app


SOURCES += main.cpp \
    GraphicsWindowQt5.cpp

HEADERS += \
    GraphicsWindowQt5.h

OTHER_FILES += \
    README.md


win32 {
    OSG_ROOT=D:\Developer\vs2015\x86_64\packages\OpenSceneGraph-3.4.0

    INCLUDEPATH += $$OSG_ROOT/include
    LIBS += -L$$OSG_ROOT/lib

    CONFIG(debug, debug|release) {
        LIBS += -losgd -losgUtild -losgGAd -losgDBd -losgViewerd -losgTextd -losgShadowd -lOpenThreadsd
    } else {
        LIBS += -losg -losgUtil -losgGA -losgDB -losgViewer -losgText -losgShadow -lOpenThreads
    }

    LIBS += -lOpenGL32
}
