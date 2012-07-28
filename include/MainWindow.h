#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTreeWidgetItem>
#include <QProgressBar>
#include <QSettings>
#include <QComboBox>
#include <QSlider>
#include <QToolButton>
#include <QLabel>
#include "OgreWidget.h"
#include "ImageViewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    OgreWidget* mOgreWidget;
    ImageViewer* mHeightmapViewer;
    QTimer* mRenderTimer;
    QProgressBar* mStatusProgressBar;
    QSettings* mSettings;

    // Widgets for context toolbar
    QToolButton* mSelectedToolButton;
    QLabel* mBrushSizeLabel; // Needs to be here so it can be disabled and re-enabled later on
    QSlider* mBrushSizeSlider;
    QLabel* mBrushWeightLabel;
    QSlider* mBrushWeightSlider;

    // Texture settings - the same widgets are reused for all terrain texture layers so the data must be stored somewhere other than just in the widget
    // All variables in here corrospond to the same internal Ogre ones
    struct TerrainTextureLayer
    {
        int worldSize;
        double minHeight;
        double fadeDist;
        QString diffuseSpecular;
        QString normalHeight;
    } mLayers[3];
    unsigned int iCurrentLayerIndex; // Keeps track of which texture is selected in the GUI. Necessary because the signal which processes the itemChanged() event from the texture selection boxs needs to know which one was selected BEFORE the event took place.

    QString mApplicationDataDir;

    // Called when the program is first run; creates a config file with default settings
    void createNewConfigFile();

    bool bWPressed;
    bool bAPressed;
    bool bSPressed;
    bool bDPressed;
    bool bQPressed;
    bool bEPressed;
    bool bShiftPressed;

    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);

public slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void editUndo();
    void editRedo();
    void selectTool();
    void addEntTool();
    void paintTool();
    void extrudeTool();
    void intrudeTool();
    void setBrushSize(int size);
    void setBrushWeight(int weight);
    void viewWireframe();
    void viewSolid();
    void screenshot();
    void generateTerrain();
    void loadTerrain();
    void clearTerrain();
    void updateTextures();
    void resetDefaultTextures();
    void textureSelected(int);
    void showHeightmapImage();
    void randomiseTerrainSeed();
    void showAboutBox();
    void options();
    void statusTextureUpdateInProgress();
    void statusTextreUpdateFinished();
    void updateSettings();
    void loadImage();
    void updateEnvironment();
    void resetDefaultEnvironment();
    void updateFogButtonColour();
    void fogModeChanged(int);
};

#endif // MAINWINDOW_H
