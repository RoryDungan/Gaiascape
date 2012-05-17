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
    src/algorithms/terrainBlock.cpp \
    src/algorithms/globalTerrainMaster.cpp \
    src/ogre/Terrain.cpp \
    src/ogre/OgreWidget.cpp \
    src/ogre/MainWindow.cpp

HEADERS  += include/algorithms/terrainBlock.h \
    include/algorithms/random.h \
    include/algorithms/globalTerrainMaster.h \
    include/ogre/Terrain.h \
    include/ogre/OgreWidget.h \
    include/ogre/MainWindow.h

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
