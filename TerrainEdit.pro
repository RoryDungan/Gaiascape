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
    src/Terrain.cpp \
    src/OgreWidget.cpp \
    src/MainWindow.cpp \
    src/algorithms/HeightMapGen.cpp \
    src/algorithms/HMBlock.cpp

HEADERS  += include/algorithms/TerrainBlock.h \
    include/algorithms/Random.h \
    include/Terrain.h \
    include/OgreWidget.h \
    include/MainWindow.h \
    src/algorithms/HeightMapGen.h \
    include/algorithms/HeightMapGen.h \
    include/algorithms/HMBlock.h

INCLUDEPATH += include/ \
    include/OGRE/ \
    ui/

UI_DIR = ui/

FORMS    += ui/MainWindow.ui

LIBS += -L. -lOgreMain -lOgreTerrain

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
