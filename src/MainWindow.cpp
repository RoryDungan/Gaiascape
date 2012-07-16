#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageViewer.h"
#include "algorithms/random.h"
#include "OptionsDialog.h"
#include "AboutBox.h"
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>
#include <QDesktopServices>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Show loading dialog


    // init variables
    bWPressed = false;
    bAPressed = false;
    bSPressed = false;
    bDPressed = false;
    bQPressed = false;
    bEPressed = false;
    bShiftPressed = false;

    ui->setupUi(this);

    // Locate applications settings directory
    mApplicationDataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    // some platforms currently don't define this location and you don't want to write to /foo
    if (mApplicationDataDir.isEmpty())
        mApplicationDataDir = QDir::homePath() + "/." + QCoreApplication::applicationName();
    mApplicationDataDir.append(QDir::separator());
    // the first time I am saving data
    if (!QFile::exists(mApplicationDataDir))
    {
        QDir dir;
        dir.mkpath(mApplicationDataDir);
    }


    qDebug() << mApplicationDataDir;

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
    mOgreWidget->setInteractionMode(OgreWidget::IM_SELECT);
    mOgreWidget->setCameraInverted(mSettings->value("Renderer/CameraInverted", false).toBool());
    //mOgreWidget->setFOVy(mSettings->value("Renderer/FOV", 45).toInt());

    // tabify dock widgets
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::East);
    tabifyDockWidget(ui->terrainDockWidget, ui->texturesDockWidget);
    tabifyDockWidget(ui->texturesDockWidget, ui->environmentDockWidget);
    tabifyDockWidget(ui->environmentDockWidget, ui->foliageDockWidget);
    ui->terrainDockWidget->raise();

    ui->texturesTreeWidget->expandAll();
    QColor colour = QColor(230, 230, 230);
    ui->fogColourLabel->setText(colour.name());
    ui->fogColourLabel->setPalette(QPalette(colour));
    ui->fogColourLabel->setAutoFillBackground(true);

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

    // Set up toolbar
    mToolComboBox = new QComboBox(this);
    ui->contextToolBar->addWidget(mToolComboBox);

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
}

MainWindow::~MainWindow()
{
    // Save settings
    mSettings->setValue("MainWindow/Maximized", this->isMaximized());

    // Q_OBJECTs are deleted automaticly

    delete mRenderTimer;
    delete mOgreWidget;
    delete mStatusProgressBar;
    delete mToolComboBox;
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
    // TODO: Ask the user if they want to save changes
    clearTerrain();
    // Delete all other data
}

void MainWindow::fileOpen()
{

}

void MainWindow::fileSave()
{

}

void MainWindow::fileSaveAs()
{

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
}

void MainWindow::addEntTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_PLACEOBJ);
}

void MainWindow::paintTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_PAINT);
}

void MainWindow::extrudeTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_EXTRUDE);
}

void MainWindow::intrudeTool()
{
    mOgreWidget->setInteractionMode(OgreWidget::IM_INTRUDE);
}

void MainWindow::viewSolid()
{
    mOgreWidget->setViewMode(Ogre::PM_SOLID);
}

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

void MainWindow::generateTerrain()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QElapsedTimer timer;
    mOgreWidget->getTerrain()->clearTerrain();
    timer.start();
    mOgreWidget->getTerrain()->generateTerrain();
    qDebug() << "Terrain genarated in" << timer.elapsed() << "milliseconds";
    QApplication::restoreOverrideCursor();
}

void MainWindow::loadTerrain()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mOgreWidget->getTerrain()->clearTerrain();
    mOgreWidget->getTerrain()->loadHeightmap();
    QApplication::restoreOverrideCursor();
}

void MainWindow::clearTerrain()
{
    mOgreWidget->getTerrain()->clearTerrain();
    mOgreWidget->getTerrain()->createFlatTerrain();
}

// Slot fo when the user changes one of the texture properties
void MainWindow::texturePropertyChanged(QTreeWidgetItem* item, int itemNum)
{
    // Only second level items need to do things (first level items do not have parents)
    if(item->parent())
    {
        Terrain::textureCatagory texCat;
        if(item->parent()->text(0) == "Grass")
            texCat = Terrain::TEX_GRASS;
        else if(item->parent()->text(0) == "Dirt")
            texCat = Terrain::TEX_DIRT;
        else if(item->parent()->text(0) == "Rock")
            texCat = Terrain::TEX_ROCK;
        else texCat = Terrain::TEX_GENERIC;

        if(item->text(0) == "Diffuse")
        {
            QString filename = QFileDialog::getOpenFileName(this, tr("Open texture file"), item->text(1), tr("Images (*.jpg *.jpeg *.jpe *.png *.tga *.bmp *.raw *.gif *.dds);;JPEG image (*.jpg *.jpeg *.jpe);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp);;RAW image (*.raw);;GIF image (*.gif);;DirectDraw surface (*.dds)"));
            if(filename != 0)
            {
                // Convert path to relative
                filename = QDir::current().relativeFilePath(filename);
                // Set texture
                mOgreWidget->getTerrain()->setTexture(Terrain::TT_DIFFUSE, texCat, filename.toStdString());
                item->setText(1, filename);
            }
        }
        else if(item->text(0) == "Normal map")
        {
            QString filename = QFileDialog::getOpenFileName(this, tr("Open texture file"), item->text(1), tr("Images (*.jpg *.jpeg *.jpe *.png *.tga *.bmp *.raw *.gif *.dds);;JPEG image (*.jpg *.jpeg *.jpe);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp);;RAW image (*.raw);;GIF image (*.gif);;DirectDraw surface (*.dds)"));
            if(filename != 0)
            {
                // Convert path to relative
                filename = QDir::current().relativeFilePath(filename);
                // Set texture
                mOgreWidget->getTerrain()->setTexture(Terrain::TT_DIFFUSE, texCat, filename.toStdString());
                item->setText(1, filename);
            }
        }
        else if(item->text(0) == "Placement height")
        {
            int height = QInputDialog::getInt(this, tr("Placement height"), tr("Placement height"), item->text(1).toInt());
            if(height)
            {
                mOgreWidget->getTerrain()->setTexturePlacementHeight(texCat, height);
                QString str;
                item->setText(1, str.setNum(height));
            }
        }
    }
}

// Create a new dialog and show the heightmap image in it
void MainWindow::showHeightmapImage()
{
    ImageViewer* iview = new ImageViewer(this, QDesktopServices::storageLocation(QDesktopServices::TempLocation) + QDir::separator() + "gaiascape-heightmap.bmp"); // Todo: make it copy the image directly from mmory rather than reading from a file
    iview->show();
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
    ui->generateTerrain->setEnabled(false);
    ui->loadTerrain->setEnabled(false);
    ui->clearTerrain->setEnabled(false);
}

void MainWindow::statusTextreUpdateFinished()
{
    statusBar()->showMessage(tr("Finished generating terrain."));
    mStatusProgressBar->setVisible(false);
    ui->generateTerrain->setEnabled(true);
    ui->loadTerrain->setEnabled(true);
    ui->clearTerrain->setEnabled(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::updateSettings()
{
    mOgreWidget->setCameraInverted(mSettings->value("Renderer/CameraInverted", false).toBool());
    mOgreWidget->setFOVy(mSettings->value("Renderer/FOV", 45).toInt());
}

void MainWindow::updateTextures()
{

}

void MainWindow::resetDefaultTextures()
{

}

void MainWindow::updateEnvironment()
{
    mOgreWidget->setSkyDome(ui->skydomeLineEdit->text(),
                            ui->skyCurvatureSpinBox->value(),
                            ui->skydomeTilingSpinBox->value());
    mOgreWidget->setFog(ui->fogModeComboBox->currentIndex(),
                        ui->fogColourLabel->palette().color(QPalette::Button),
                        ui->fogDensitySpinBox->value(),
                        ui->fogStartSpinBox->value(),
                        ui->fogEndSpinBox->value());
}

void MainWindow::resetDefaultEnvironment()
{
    // Reset all controls to defaults
    ui->skydomeLineEdit->setText("media/textures/clouds.jpg");
    ui->skyCurvatureSpinBox->setValue(10.0);
    ui->skydomeTilingSpinBox->setValue(8);
    ui->fogModeComboBox->setCurrentIndex(0);
    QColor colour = QColor(230, 230, 230);
    ui->fogColourLabel->setText(colour.name());
    ui->fogColourLabel->setPalette(QPalette(colour));
    ui->fogDensitySpinBox->setValue(0.001);
    ui->fogStartSpinBox->setValue(0);
    ui->fogEndSpinBox->setValue(0);

    updateEnvironment();
}

void MainWindow::loadSkydomeImage()
{
    // Get file path
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open texture file"), QString(), tr("Images (*.jpg *.jpeg *.jpe *.png *.tga *.bmp *.raw *.gif *.dds);;JPEG image (*.jpg *.jpeg *.jpe);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp);;RAW image (*.raw);;GIF image (*.gif);;DirectDraw surface (*.dds)"));

    // Return if no file was selected
    if(filePath == 0) return;

    // Convert path to relative
    filePath = QDir::current().relativeFilePath(filePath);

    /*/ Depracated - Find the appropriate lineEdit box and update it with the new file
    if(sender() == ui->skyboxBackButton)
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

    ui->skydomeLineEdit->setText(filePath);
}

void MainWindow::updateFogButtonColour()
{
    // Open a new colour selection dialog, starting with the currently selected colour
    QColor colour = QColorDialog::getColor(ui->fogColourLabel->palette().color(QPalette::Button), this);
    if(colour.isValid())
    {
        ui->fogColourLabel->setText(colour.name());
        ui->fogColourLabel->setPalette(QPalette(colour));
    }
}

void MainWindow::fogModeChanged(int mode)
{
    switch(mode)
    {
    case 0: // Disabled - disable all fog controls
        ui->fogColourButton->setEnabled(false);
        ui->fogColourLabel->setEnabled(false);
        ui->fogColourLabel_0->setEnabled(false);
        ui->fogDensityLabel->setEnabled(false);
        ui->fogDensitySpinBox->setEnabled(false);
        ui->fogEndLabel->setEnabled(false);
        ui->fogEndSpinBox->setEnabled(false);
        ui->fogStartLabel->setEnabled(false);
        ui->fogStartSpinBox->setEnabled(false);
        break;
    case 1: // Linear fog
        ui->fogColourButton->setEnabled(true);
        ui->fogColourLabel->setEnabled(true);
        ui->fogColourLabel_0->setEnabled(true);
        ui->fogDensityLabel->setEnabled(false);
        ui->fogDensitySpinBox->setEnabled(false);
        ui->fogEndLabel->setEnabled(true);
        ui->fogEndSpinBox->setEnabled(true);
        ui->fogStartLabel->setEnabled(true);
        ui->fogStartSpinBox->setEnabled(true);
        break;
    default: // Exponential fog
        ui->fogColourButton->setEnabled(true);
        ui->fogColourLabel->setEnabled(true);
        ui->fogColourLabel_0->setEnabled(true);
        ui->fogDensityLabel->setEnabled(true);
        ui->fogDensitySpinBox->setEnabled(true);
        ui->fogEndLabel->setEnabled(false);
        ui->fogEndSpinBox->setEnabled(false);
        ui->fogStartLabel->setEnabled(false);
        ui->fogStartSpinBox->setEnabled(false);
        break;
    }
}
