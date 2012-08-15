#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "algorithms/random.h"
#include "OptionsDialog.h"
#include "AboutBox.h"
#include "NewFileWizard.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>
#include <QDesktopServices>
#include <QColorDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // init variables
    bWPressed = false;
    bAPressed = false;
    bSPressed = false;
    bDPressed = false;
    bQPressed = false;
    bEPressed = false;
    bShiftPressed = false;
    bSaved = true;
    filepath = "";
    // temp
    counter = 1;

    ui->setupUi(this);

    // Locate applications settings directory
    mApplicationDataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    // some platforms currently don't define this location and you don't want to write to /foo
    if (mApplicationDataDir.isEmpty())
        mApplicationDataDir = QDir::homePath() + "/." + QCoreApplication::applicationName();
    mApplicationDataDir.append(QDir::separator());
    // the first time we are saving data
    if (!QFile::exists(mApplicationDataDir))
    {
        QDir dir;
        dir.mkpath(mApplicationDataDir);
    }

    // Load user preferences
    mSettings = new QSettings(mApplicationDataDir + "settings.ini", QSettings::IniFormat, this);

    if(!mSettings->contains("Renderer/RenderingSubsystem"))
#ifdef _WIN32 // Default to Direct3D on Windows, OpenGL on everything else
        mSettings->setValue("Renderer/RenderingSubsystem", "Direct3D9");
#else
        mSettings->setValue("Renderer/RenderingSubsystem", "OpenGL");
#endif

    mOgreWidget = new OgreWidget(this, mApplicationDataDir + "ogre.log", mSettings->value("Renderer/RenderingSubsystem").toString());

    setCentralWidget(mOgreWidget);
    mOgreWidget->setFocus();
    mOgreWidget->setCameraInverted(mSettings->value("Renderer/CameraInverted", false).toBool());
    //mOgreWidget->setFOVy(mSettings->value("Renderer/FOV", 45).toInt());

    // tabify dock widgets
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::East);
    tabifyDockWidget(ui->terrainDockWidget, ui->texturesDockWidget);
    tabifyDockWidget(ui->texturesDockWidget, ui->environmentDockWidget);
    tabifyDockWidget(ui->environmentDockWidget, ui->foliageDockWidget);
    ui->terrainDockWidget->raise();
    randomiseTerrainSeed();

    QColor colour = QColor(230, 230, 230);
    ui->sunColourLabel->setText(colour.name());
    ui->sunColourLabel->setPalette(QPalette(colour));
    ui->sunColourLabel->setAutoFillBackground(true);

    // Set up action group for tool buttons
    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->addAction(ui->actionSelect);
    toolGroup->addAction(ui->actionPlace_entities);
    toolGroup->addAction(ui->actionPaint);
    toolGroup->addAction(ui->actionExtrude);
    toolGroup->addAction(ui->actionIntrude);

    QActionGroup* viewModeGroup = new QActionGroup(this);
    viewModeGroup->addAction(ui->action_Solid);
    viewModeGroup->addAction(ui->action_Wireframe);

    // Add actions to the main toolbar with correct style
    QToolButton* screenshotButton = new QToolButton(this);
    screenshotButton->setDefaultAction(ui->actionTake_screenshot);
    screenshotButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->fileToolBar->addSeparator();
    ui->fileToolBar->addWidget(screenshotButton);
    QToolButton* heightmapButton = new QToolButton(this);
    heightmapButton->setDefaultAction(ui->actionShow_heightmap_image);
    heightmapButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->fileToolBar->addWidget(heightmapButton);

    // Set up context toolbar
    // Create button for selected tool, with a menu for choosing tools
    mSelectedToolButton = new QToolButton(this);
    mSelectedToolButton->setToolTip(tr("Selected tool"));
    mSelectedToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    mSelectedToolButton->setPopupMode(QToolButton::InstantPopup);
    QMenu* menu = new QMenu(this);
    // Get menu from the existing action group
    QList<QAction*> actionList = toolGroup->actions();
    foreach(QAction* act, actionList)
        menu->addAction(act);
    mSelectedToolButton->setMenu(menu);
    ui->contextToolBar->addWidget(mSelectedToolButton);
    ui->contextToolBar->addSeparator();
    // Brush properties
    // Create label, then add the slider
    mBrushSizeLabel = new QLabel(tr("Brush size:"), this);
    ui->contextToolBar->addWidget(mBrushSizeLabel);
    mBrushSizeSlider = new QSlider(Qt::Horizontal, this);
    mBrushSizeSlider->setFixedWidth(60);
    ui->contextToolBar->addWidget(mBrushSizeSlider);
    // Make the Ogre widget update when the slider is moved
    connect(mBrushSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrushSize(int)));
    ui->contextToolBar->addSeparator();
    // Same as above, this time for the brush weight slider
    mBrushWeightLabel = new QLabel(tr("Brush weight:"), this);
    ui->contextToolBar->addWidget(mBrushWeightLabel);
    mBrushWeightSlider = new QSlider(Qt::Horizontal, this);
    mBrushWeightSlider->setFixedWidth(60);
    ui->contextToolBar->addWidget(mBrushWeightSlider);
    connect(mBrushWeightSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrushWeight(int)));

    selectTool(); // This should be the default tool when starting up. Calling this function tells mOgreWidget we want select mode and sets up the context toolbar correctly

    // Set up multiple resolutions for icons
    // The designer .ui file does not support having multiple resolution icons for a single
    // QAction, so we must set up icons manually for any QActions which appear in both the
    // menu and the toolbar
    QIcon newIcon;
    newIcon.addFile(":/media/16x16/document-new.png");
    newIcon.addFile(":/media/24x24/document-new.png");
    ui->action_New->setIcon(newIcon);
    QIcon openIcon;
    openIcon.addFile(":/media/16x16/document-open.png");
    openIcon.addFile(":/media/24x24/document-open.png");
    ui->action_Open->setIcon(openIcon);
    QIcon saveIcon;
    saveIcon.addFile(":/media/16x16/document-save.png");
    saveIcon.addFile(":/media/24x24/document-save.png");
    ui->action_Save->setIcon(saveIcon);
    QIcon undoIcon;
    undoIcon.addFile(":/media/16x16/edit-undo.png");
    undoIcon.addFile(":/media/24x24/edit-undo.png");
    ui->action_Undo->setIcon(undoIcon);
    QIcon redoIcon;
    redoIcon.addFile(":/media/16x16/edit-redo.png");
    redoIcon.addFile(":/media/24x24/edit-redo.png");
    ui->action_Redo->setIcon(redoIcon);
    QIcon screenshotIcon;
    screenshotIcon.addFile(":/media/16x16/camera-photo.png");
    screenshotIcon.addFile(":/media/24x24/camera-photo.png");
    ui->actionTake_screenshot->setIcon(screenshotIcon);
    QIcon imageIcon;
    imageIcon.addFile(":/media/16x16/image.png");
    imageIcon.addFile(":/media/24x24/image.png");
    ui->actionShow_heightmap_image->setIcon(imageIcon);

    // Set up heightmap viewer
    mHeightmapViewer = new ImageViewer(this);

    // Set up textures panel - this will be removed once we get the program to load a template
    iCurrentLayerIndex = 0;
    resetDefaultTextures();
    ui->textureSelectionList->setCurrentRow(0);

    // Set up progress bar
    mStatusProgressBar = new QProgressBar(this);
    mStatusProgressBar->setVisible(false); // Should only appear when needed
    statusBar()->addPermanentWidget(mStatusProgressBar);

    // set up timer to get the Ogre widget to render at 60fps
    mRenderTimer = new QTimer(this);
    connect(mRenderTimer, SIGNAL(timeout()), mOgreWidget, SLOT(updateGL()));
    mRenderTimer->start(1000.0f/60.0f);

    // Listen for when the terrain is updating textures
    connect(mOgreWidget, SIGNAL(textureUpdateInProgress()), this, SLOT(statusTextureUpdateInProgress()));
    connect(mOgreWidget, SIGNAL(textureUpdateFinished()), this, SLOT(statusTextreUpdateFinished()));

    // Ready for work
    statusBar()->showMessage(tr("Ready"));

    if(mSettings->value("MainWindow/Maximized", false).toBool()) this->showMaximized();

    ui->updateEnvironmentButton->setEnabled(false);
    ui->updateTerrainButton->setEnabled(false);
    ui->updateTexturesButton->setEnabled(false);

    ui->groupBox_3->setVisible(false);
}

MainWindow::~MainWindow()
{
    // Save settings
    mSettings->setValue("MainWindow/Maximized", this->isMaximized());

    // Clean up temporty files
    QFile::remove(QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "gaiascape-heightmap.bmp");

    delete mHeightmapViewer;

    // Q_OBJECTs are deleted automaticly

    delete mRenderTimer;
    delete mOgreWidget;
    delete mStatusProgressBar;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        mOgreWidget->bCtrlPressed = true;
        break;
    case Qt::Key_W:
        bWPressed = true;
        break;
    case Qt::Key_A:
        bAPressed = true;
        break;
    case Qt::Key_S:
        bSPressed = true;
        break;
    case Qt::Key_D:
        bDPressed = true;
        break;
    case Qt::Key_Q:
        bQPressed = true;
        break;
    case Qt::Key_E:
        bEPressed = true;
        break;
    case Qt::Key_Shift:
        bShiftPressed = true;
        break;
    default:
        break;
    }
    mOgreWidget->setCameraMovementDirection(bWPressed, bSPressed, bAPressed, bDPressed, bQPressed, bEPressed, bShiftPressed);
}

void MainWindow::keyReleaseEvent(QKeyEvent * event)
{
    switch(event->key())
    {
    case Qt::Key_Control:
        mOgreWidget->bCtrlPressed = false;
        break;
    case Qt::Key_W:
        bWPressed = false;
        break;
    case Qt::Key_A:
        bAPressed = false;
        break;
    case Qt::Key_S:
        bSPressed = false;
        break;
    case Qt::Key_D:
        bDPressed = false;
        break;
    case Qt::Key_Q:
        bQPressed = false;
        break;
    case Qt::Key_E:
        bEPressed = false;
        break;
    case Qt::Key_Shift:
        bShiftPressed = false;
        break;
    default:
        break;
    }
    mOgreWidget->setCameraMovementDirection(bWPressed, bSPressed, bAPressed, bDPressed, bQPressed, bEPressed, bShiftPressed);
}

// Slots
void MainWindow::fileNew()
{
    if(!bSaved)
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, tr("Save"),
                                      tr("Do you want to save changes?"),
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            fileSave();
        else if (reply == QMessageBox::Cancel)
            return;
        // if reply == No, continue
    }

    // Create new terrain from wizard
    NewFileWizard* wizard = new NewFileWizard(this);
    connect(wizard, SIGNAL(createdFile(WorldOptions)), this, SLOT(loadFromWorldOptions(WorldOptions)));
    wizard->show();

    bSaved = false;
}

void MainWindow::fileOpen()
{
    QString tmppath = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Gaiascape files (*.gsw);;All files (*.*)"));
    if(tmppath.isNull()) return;
    else filepath = tmppath;
    loadWorldOptionsFromFile(filepath);
}

void MainWindow::fileSave()
{
    if(filepath.isEmpty()) fileSaveAs();
    else
    {
        //saveWorldOptionsToFile();
    }
}

void MainWindow::fileSaveAs()
{
    QString tmppath = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Gaiascape files (*.gsw)"));
    if(tmppath.isNull()) return;
    else filepath = tmppath;
    fileSave();
}

void MainWindow::editUndo()
{

}

void MainWindow::editRedo()
{

}

void MainWindow::selectTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_SELECT);

    // Update context toobar
    mSelectedToolButton->setIcon(ui->actionSelect->icon());
    mSelectedToolButton->setText(ui->actionSelect->text());
    mBrushSizeLabel->setEnabled(false);
    mBrushSizeSlider->setEnabled(false);
    mBrushWeightLabel->setEnabled(false);
    mBrushWeightSlider->setEnabled(false);
}

void MainWindow::addEntTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_PLACEOBJ);

    // Update context toobar button
    mSelectedToolButton->setIcon(ui->actionPlace_entities->icon());
    mSelectedToolButton->setText(ui->actionPlace_entities->text());
    mBrushSizeLabel->setEnabled(false);
    mBrushSizeSlider->setEnabled(false);
    mBrushWeightLabel->setEnabled(false);
    mBrushWeightSlider->setEnabled(false);
}

void MainWindow::paintTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_PAINT);

    // Update context toobar button
    mSelectedToolButton->setIcon(ui->actionPaint->icon());
    mSelectedToolButton->setText(ui->actionPaint->text());
    mBrushSizeLabel->setEnabled(true);
    mBrushSizeSlider->setEnabled(true);
    mBrushWeightLabel->setEnabled(true);
    mBrushWeightSlider->setEnabled(true);
}

void MainWindow::extrudeTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_EXTRUDE);

    // Update context toobar button
    mSelectedToolButton->setIcon(ui->actionExtrude->icon());
    mSelectedToolButton->setText(ui->actionExtrude->text());
    mBrushSizeLabel->setEnabled(true);
    mBrushSizeSlider->setEnabled(true);
    mBrushSizeSlider->setSliderPosition((int)(mOgreWidget->getBrushSize() * 500));
    mBrushWeightLabel->setEnabled(true);
    mBrushWeightSlider->setEnabled(true);
    mBrushWeightSlider->setSliderPosition((int)(mOgreWidget->getBrushWeight() / 5));
}

void MainWindow::intrudeTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_INTRUDE);

    // Update context toobar button
    mSelectedToolButton->setIcon(ui->actionIntrude->icon());
    mSelectedToolButton->setText(ui->actionIntrude->text());
    mBrushSizeLabel->setEnabled(true);
    mBrushSizeSlider->setEnabled(true);
    mBrushSizeSlider->setSliderPosition((int)(mOgreWidget->getBrushSize() * 500));
    mBrushWeightLabel->setEnabled(true);
    mBrushWeightSlider->setEnabled(true);
    mBrushWeightSlider->setSliderPosition((int)(mOgreWidget->getBrushWeight() / 5));
}

// Update the Ogre widget when the brush size slider is moved.
// This adjusts how big an area the selected brush will affect
void MainWindow::setBrushSize(int size)
{
    // The slider goes form 0 to 99 but acceptable values for brush size range from 0.0 to 0.2
    mOgreWidget->setBrushSize((float)size / 500);
}

// Update the Ogre widget when the beush weight slider is moved.
// This adjusts how much effect the selected brush will have
void MainWindow::setBrushWeight(int weight)
{
    // Slider goes from 0 to 99, reasnoble values for brush weight are up to around 500, hence why we multiply it by 5
    mOgreWidget->setBrushWeight((float)weight * 5);
}

// Set the 3d view to solid, textures poligons
void MainWindow::viewSolid()
{
    mOgreWidget->setViewMode(Ogre::PM_SOLID);
}

// Set the 3d view mode to wireframe
void MainWindow::viewWireframe()
{
    mOgreWidget->setViewMode(Ogre::PM_WIREFRAME);
}

// Save an image of the current render output to a file
void MainWindow::screenshot()
{
    mOgreWidget->saveScreenshotToFile(QFileDialog::getSaveFileName(this, tr("Save Image"),
                                             "", tr("PNG image (*.png);;JPEG Image (*.jpg *.jpeg *.jpe);;Targa image (*.tga);;Bitmap image (*.bmp)")));
}

void MainWindow::updateTerrain()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QElapsedTimer timer;
    mOgreWidget->getTerrain()->clearTerrain();
    timer.start();
    // This can be spawned at any point. If this function does what I believe it does, later this will be modifyed to create
    // multiple terrain blocks.
    mOgreWidget->getTerrain()->generateTerrain(ui->randomSeedBox->value(), 8, ui->terrainScaleSlider->value(), ui->terrainErosionSlider->value(), ui->randomFactorSlider->value(), ui->treeDensitySlider->value());
    qDebug() << "Terrain genarated in" << timer.elapsed() << "milliseconds";
    QApplication::restoreOverrideCursor();

    ui->updateTerrainButton->setEnabled(false);
}

void MainWindow::generateTerrain()
{
    randomiseTerrainSeed();
    // reset everything
    updateTerrain();
}

void MainWindow::loadTerrain()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Import heightmap image"), "",
                                                    tr("PNG image (*.png);;JPEG Image (*.jpg *.jpeg *.jpe);;Targa image (*.tga);;Bitmap image (*.bmp)"));
    if(filePath.isEmpty()) return; // Cancel if no path was selected
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mOgreWidget->getTerrain()->clearTerrain();
    mOgreWidget->getTerrain()->loadHeightmap(filePath.toStdString());
    QApplication::restoreOverrideCursor();
}

void MainWindow::clearTerrain()
{
    mOgreWidget->getTerrain()->clearTerrain();
    mOgreWidget->getTerrain()->createFlatTerrain();
}

// Create a new dialog and show the heightmap image in it
void MainWindow::showHeightmapImage()
{
    QImage image(QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "gaiascape-heightmap.bmp");
    if(!image.isNull()) mHeightmapViewer->loadImage(QPixmap::fromImage(image)); // Todo: make it copy the image directly from mmory rather than reading from a file
    mHeightmapViewer->showNormal();
}

//
void MainWindow::reloadHeightmap()
{
    mHeightmapViewer->loadImage(QPixmap::fromImage(mOgreWidget->getTerrain()->getQImage(0, 0)));
}

// Create a new random seed value
void MainWindow::randomiseTerrainSeed()
{
    ui->randomSeedBox->setValue(Random::getSingleton().getRand(0, 2147483647));
}

// Show configuration window
void MainWindow::options()
{
    OptionsDialog* dialog = new OptionsDialog(this, mSettings);
    dialog->show();
    dialog->setModal(true);

    connect(dialog, SIGNAL(settingsUpdated()), this, SLOT(updateSettings()));
}

// Show about box wit informaton on copyright and credits
void MainWindow::showAboutBox()
{
    AboutBox* aboutBox = new AboutBox(this);
    aboutBox->show();
    aboutBox->setModal(true);
}

void MainWindow::statusTextureUpdateInProgress()
{
    statusBar()->showMessage(tr("Updating textures, please wait..."));
    mStatusProgressBar->setVisible(true);
    mStatusProgressBar->setMinimum(0);
    mStatusProgressBar->setMaximum(0);

    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

    // Creating a new terrain while the program is updating the textures causes it to freeze and stop responding untl he texture updae has finished
    // This can givethe impression that the program has crashed, so it is prevented by disabling the buttons to create new terrains temporarily
    //ui->updateTerrainButton->setEnabled(false);
    ui->loadTerrainButton->setEnabled(false);
    ui->clearTerrainButton->setEnabled(false);
}

void MainWindow::statusTextreUpdateFinished()
{
    statusBar()->showMessage(tr("Finished generating terrain."));
    mStatusProgressBar->setVisible(false);
    //ui->updateTerrainButton->setEnabled(true);
    ui->loadTerrainButton->setEnabled(true);
    ui->clearTerrainButton->setEnabled(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::updateSettings()
{
    mOgreWidget->setCameraInverted(mSettings->value("Renderer/CameraInverted", false).toBool());
    mOgreWidget->setFOVy(mSettings->value("Renderer/FOV", 45).toInt());
}

void MainWindow::updateTextures()
{
    // First save all texture data
    mLayers[iCurrentLayerIndex].diffuseSpecular = ui->diffuseSpecularLineEdit->text();
    mLayers[iCurrentLayerIndex].normalHeight = ui->normalHeightLineEdit->text();
    mLayers[iCurrentLayerIndex].worldSize = ui->textureSizeBox->value();
    mLayers[iCurrentLayerIndex].fadeDist = ui->texturePlacementHeightBox->value();

    // Update all textures
    for(unsigned int i = 0; i < 3; i++)
        mOgreWidget->getTerrain()->replaceTexture(i, mLayers[i].worldSize, mLayers[i].diffuseSpecular.toStdString(), mLayers[i].normalHeight.toStdString());

    ui->updateTexturesButton->setEnabled(false);
}

void MainWindow::resetDefaultTextures()
{
    // Replace textures with defaults
    mLayers[0].diffuseSpecular = "dirt_grayrocky_diffusespecular.dds";
    mLayers[0].normalHeight = "dirt_grayrocky_normalheight.dds";
    mLayers[0].worldSize = 100;
    mLayers[1].diffuseSpecular = "grass_green-01_diffusespecular.dds";
    mLayers[1].normalHeight = "grass_green-01_normalheight.dds";
    mLayers[1].worldSize = 30;
    mLayers[2].diffuseSpecular = "growth_weirdfungus-03_diffusespecular.dds";
    mLayers[2].normalHeight = "growth_weirdfungus-03_normalheight.dds";
    mLayers[2].worldSize = 200;

    // Update GUI
    ui->diffuseSpecularLineEdit->setText(mLayers[iCurrentLayerIndex].diffuseSpecular);
    ui->normalHeightLineEdit->setText(mLayers[iCurrentLayerIndex].normalHeight);
    ui->textureSizeBox->setValue(mLayers[iCurrentLayerIndex].worldSize);
    ui->texturePlacementHeightBox->setValue(mLayers[iCurrentLayerIndex].fadeDist);

    // Update textures - resetting the default textures should just reset the GUI
    //for(unsigned int i = 0; i < 3; i++)
    //    mOgreWidget->getTerrain()->replaceTexture(i, mLayers[i].worldSize, mLayers[i].diffuseSpecular.toStdString(), mLayers[i].normalHeight.toStdString());
}

// Update the rest of the Textures panel when a different one is selected
void MainWindow::textureSelected(int id)
{
    // First save data for the old texture
    mLayers[iCurrentLayerIndex].diffuseSpecular = ui->diffuseSpecularLineEdit->text();
    mLayers[iCurrentLayerIndex].normalHeight = ui->normalHeightLineEdit->text();
    mLayers[iCurrentLayerIndex].worldSize = ui->textureSizeBox->value();
    mLayers[iCurrentLayerIndex].fadeDist = ui->texturePlacementHeightBox->value();

    // Then set up the new one
    ui->diffuseSpecularLineEdit->setText(mLayers[id].diffuseSpecular);
    ui->normalHeightLineEdit->setText(mLayers[id].normalHeight);
    ui->textureSizeBox->setValue(mLayers[id].worldSize);
    ui->texturePlacementHeightBox->setValue(mLayers[id].fadeDist);

    iCurrentLayerIndex = id;

    mOgreWidget->setLayerEdit(id);
}

void MainWindow::updateEnvironment()
{
    mOgreWidget->setSkyDome(ui->skydomeLineEdit->text(),
                            ui->skyCurvatureSpinBox->value(),
                            ui->skydomeTilingSpinBox->value());
    mOgreWidget->setSunPosition(ui->sunAltitudeSpinBox->value(),
                                ui->sunAngleSpinBox->value());
    ui->updateEnvironmentButton->setEnabled(false);

}

void MainWindow::resetDefaultEnvironment()
{
    // Reset all controls to defaults
    ui->skydomeLineEdit->setText("media/textures/clouds.jpg");
    ui->skyCurvatureSpinBox->setValue(10.0);
    ui->skydomeTilingSpinBox->setValue(8);

    updateEnvironment();
}

// Generic function updating the text in all the cases where there is a line edit box with a button for selecting a path next to it
void MainWindow::loadImage()
{
    // Get file path
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open texture file"), QString(), tr("Images (*.jpg *.jpeg *.jpe *.png *.tga *.bmp *.raw *.gif *.dds);;JPEG image (*.jpg *.jpeg *.jpe);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp);;RAW image (*.raw);;GIF image (*.gif);;DirectDraw surface (*.dds)"));

    // Return if no file was selected
    if(filePath == 0) return;

    // Convert path to relative
    filePath = QDir::current().relativeFilePath(filePath);

    // Find the appropriate lineEdit box and update it with the new file
    if(sender() == ui->skydomeButton)
        ui->skydomeLineEdit->setText(filePath);
    else if(sender() == ui->diffuseSpecularButton)
        ui->diffuseSpecularLineEdit->setText(filePath);
    else if(sender() == ui->normalHeightButton)
        ui->normalHeightLineEdit->setText(filePath);
    // Depracated - functions for skyboxes
    /*else if(sender() == ui->skyboxBackButton)
        ui->skyboxBackLineEdit->setText(filePath);
    else if(sender() == ui->skyboxDownButton)
        ui->skyboxDownLineEdit->setText(filePath);
    else if(sender() == ui->skyboxFrontButton)
        ui->skyboxFrontLineEdit->setText(filePath);
    else if(sender() == ui->skyboxLeftButton)
        ui->skyboxLeftLineEdit->setText(filePath);
    else if(sender() == ui->skyboxRightButton)
        ui->skyboxRightLineEdit->setText(filePath);
    else if(sender() == ui->skyboxUpButton)
        ui->skyboxUpLineEdit->setText(filePath);*/
}

void MainWindow::updateColourButton()
{
    // Open a new colour selection dialog, starting with the currently selected colour
    QColor colour;
    if(sender() == ui->sunColourButton)
        colour = QColorDialog::getColor(ui->sunColourLabel->palette().color(QPalette::Button), this);

    if(!colour.isValid())
        return;

    if(sender() == ui->sunColourButton)
    {
        ui->sunColourLabel->setText(colour.name());
        ui->sunColourLabel->setPalette(QPalette(colour));
    }
}

void MainWindow::loadFromWorldOptions(WorldOptions options)
{
    ui->randomSeedBox->setValue(options.terrainSeed);
    ui->randomFactorSlider->setValue(options.terrainRandomFactor);
    ui->terrainScaleSlider->setValue(options.terrainScale);
    ui->terrainSizeSpinBox->setValue(options.terrainSize);
    ui->terrainErosionSlider->setValue(options.erosionPasses);
    updateTerrain();
}

void MainWindow::saveWorldOptionsToFile(QString filepath, WorldOptions& options)
{

}

WorldOptions* MainWindow::loadWorldOptionsFromFile(QString filepath) {}

void MainWindow::enableUpdateEnvironment() { ui->updateEnvironmentButton->setEnabled(true); }
void MainWindow::enableUpdateTextures() { ui->updateTexturesButton->setEnabled(true); }
void MainWindow::enableUpdateTerrain() { ui->updateTerrainButton->setEnabled(true); }
void MainWindow::enableUpdateFoliage() { /* enable update foliage button */ }
