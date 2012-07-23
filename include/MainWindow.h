#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTreeWidgetItem>
#include <QProgressBar>
#include <QSettings>
#include <QComboBox>
#include <QSlider>
#include "OgreWidget.h"

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
    QTimer* mRenderTimer;
    QProgressBar* mStatusProgressBar;
    QSettings* mSettings;
    QComboBox* mToolComboBox;
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
    void viewWireframe();
    void viewSolid();
    void screenshot();
    void generateTerrain();
    void loadTerrain();
    void clearTerrain();
    void texturePropertyChanged(QTreeWidgetItem*, int);
    void updateTextures();
    void resetDefaultTextures();
    void showHeightmapImage();
    void randomiseTerrainSeed();
    void showAboutBox();
    void options();
    void statusTextureUpdateInProgress();
    void statusTextreUpdateFinished();
    void updateSettings();
    void loadSkydomeImage();
    void updateEnvironment();
    void resetDefaultEnvironment();
    void updateFogButtonColour();
    void fogModeChanged(int);
};

#endif // MAINWINDOW_H
