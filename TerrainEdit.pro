#-------------------------------------------------
#
# Project created by QtCreator 2012-02-23T23:28:27
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = TerrainEdit
TEMPLATE = app

CXXFLAGS += -fno-strict-aliasing

SOURCES += src/main.cpp\
    src/algorithms/TerrainBlock.cpp \
    src/algorithms/GlobalTerrainMaster.cpp \
    src/Terrain.cpp \
    src/OgreWidget.cpp \
    src/MainWindow.cpp

HEADERS  += include/algorithms/TerrainBlock.h \
    include/algorithms/Random.h \
    include/algorithms/GlobalTerrainMaster.h \
    include/Terrain.h \
    include/OgreWidget.h \
    include/MainWindow.h

INCLUDEPATH += include/ \
    include/OGRE/

FORMS    += ui/MainWindow.ui

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
