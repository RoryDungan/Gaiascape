#-------------------------------------------------
#
# Project created by QtCreator 2012-02-23T23:28:27
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = TerrainEdit
TEMPLATE = app

CXXFLAGS += -fno-strict-aliasing

SOURCES += main.cpp\
        MainWindow.cpp \
    OgreWidget.cpp \
    Terrain.cpp

HEADERS  += MainWindow.h \
    OgreWidget.h \
    Terrain.h

FORMS    += MainWindow.ui

LIBS += -L. -lOgreMain -lOgreTerrain

INCLUDEPATH += OGRE

win32 {
    RC_FILE = resource.rc
}

macx {
    ICON = media/icon.icns
}

OTHER_FILES += \
    resource.rc

RESOURCES += \
    icons.qrc
