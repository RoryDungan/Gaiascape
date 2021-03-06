#-------------------------------------------------
#
# Project created by QtCreator 2012-02-23T23:28:27
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = TerrainEdit
TEMPLATE = app

CXXFLAGS += -fno-strict-aliasing

Debug:DEFINES += _DEBUG

SOURCES += src/main.cpp\
    src/Terrain.cpp \
    src/OgreWidget.cpp \
    src/MainWindow.cpp \
    src/algorithms/HeightMapGen.cpp \
    src/algorithms/HMBlock.cpp \
    src/ImageViewer.cpp \
    src/OptionsDialog.cpp \
    src/flora/FloraManager.cpp \
    src/flora/FloraMain.cpp \
    src/flora/FloraTree.cpp \
    src/AddObjectDialog.cpp \
    src/NewFileWizard.cpp \
    src/WorldOptions.cpp

HEADERS  += include/algorithms/TerrainBlock.h \
    include/algorithms/Random.h \
    include/Terrain.h \
    include/OgreWidget.h \
    include/MainWindow.h \
    include/algorithms/HeightMapGen.h \
    include/algorithms/HMBlock.h \
    include/ImageViewer.h \
    include/OptionsDialog.h \
    include/FloraManager.h \
    include/FloraMain.h \
    include/FloraTree.h \
    include/AboutBox.h \
    include/AddObjectDialog.h \
    include/NewFileWizard.h \
    include/WorldOptions.h

OGRE_LOCATION = $$(OGRE_HOME)
BOOST_LOCATION = $$(BOOST_HOME)

isEmpty(OGRE_LOCATION) {
    error("Make sure to set OGRE_HOME to the location of your Ogre SDK")
}
isEmpty(BOOST_LOCATION) {
    error("Make sure to set BOOST_HOME to the location of Boost")
}

INCLUDEPATH += include/ \
    ui/ \
    $$(OGRE_HOME)/include \
    $$(OGRE_HOME)/include/OGRE \
    $$(BOOST_HOME)

UI_DIR = ui/

FORMS    += ui/MainWindow.ui \
    ui/ImageViewer.ui \
    ui/OptionsDialog.ui \
    ui/AddObjectDialog.ui

LIBS += -L$$(BOOST_HOME)/stage/lib

CONFIG(debug, debug|release) {
    LIBS += -L$$(OGRE_HOME)/lib/debug \
        -L$$(OGRE_HOME)/bin/debug \
        -lOgreMain_d \
        -lOgreTerrain_d \
        -lboost_system-mgw48-mt-d-1_60
} else {
    LIBS += -L$$(OGRE_HOME)/lib/release \
        -L$$(OGRE_HOME)/bin/release \
        -lOgreMain \
        -lOgreTerrain \
        -lboost_system-mgw48-mt-1_60
}

win32 {
    DEFINES += _WIN32
    RC_FILE = resource.rc
}

macx {
    ICON = media/icon.icns
}

OTHER_FILES += \
    resource.rc

RESOURCES += \
    icons.qrc
