#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ImageViewer.h"
#include "algorithms/random.h"
#include "OptionsDialog.h"
#include <QTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QCheckBox>
#include <QDir>

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

    // Load user preferences
    mSettings = new QSettings("settings.ini", QSettings::IniFormat, this);

    mOgreWidget = new OgreWidget;
    setCentralWidget(mOgreWidget);
    mOgreWidget->setInteractionMode(OgreWidget::IM_SELECT);
    mOgreWidget->setCameraInverted(mSettings->value("Renderer/CameraInverted", false).toBool());
    //mOgreWidget->setFOVy(mSettings->value("Renderer/FOV", 45).toInt());

    // tabify dock widgets
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::East);
    tabifyDockWidget(ui->terrainDockWidget, ui->texturesDockWidget);
    tabifyDockWidget(ui->texturesDockWidget, ui->skyDockWidget);
    tabifyDockWidget(ui->skyDockWidget, ui->foliageDockWidget);
    ui->terrainDockWidget->raise();

    ui->texturesTreeWidget->expandAll();

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
    mSettings->setValue("MainWindow/Maximized", this->isMaximized());

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
    //ui->action_Wireframe->setChecked(false);
    //ui->action_Solid->setChecked(true);
    mOgreWidget->setViewMode(Ogre::PM_SOLID);
}

void MainWindow::viewWireframe()
{
    //ui->action_Wireframe->setChecked(true);
    //ui->action_Solid->setChecked(false);
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
    mOgreWidget->getTerrain()->clearTerrain();
    mOgreWidget->getTerrain()->generateTerrain();
}

void MainWindow::loadTerrain()
{
    mOgreWidget->getTerrain()->clearTerrain();
    mOgreWidget->getTerrain()->loadHeightmap();
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
    ImageViewer* iview = new ImageViewer(this, "map.bmp"); // Todo: make it copy the image directly from mmory rather than reading from a file
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
    QDialog* Dialog = new QDialog;

    QVBoxLayout *verticalLayout = new QVBoxLayout(Dialog);
    QLabel *label = new QLabel(Dialog);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy);
    label->setWordWrap(true);

    verticalLayout->addWidget(label);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Dialog);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Close);

    verticalLayout->addWidget(buttonBox);

    Dialog->setWindowTitle(tr("About Gaiascape"));
    label->setText(tr("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
                      "<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body>\n"
                      "<h1>Gaiascape</h1>\n"
                      "<p>Written by Rory Dungan &lt;<a href=\"mailto:rorydungan@gmail.com\">rorydungan@gmail.com</a>&gt; and Dylan Ford &lt;<a href=\"mailto:dylan@fordfam.com\">dylan@fordfam.com</a>&gt;</p>\n"
                      "<p>Artwork by Daniel Galbraith &lt;<a href=\"mailto:dgalbraih2@gmail.com\">dgalbraith2@gmail.com</a>&gt;and Daniel Docherty &lt;<a href=\"mailto:ddocherty.z1@gmail.com\">ddocherty.z1@gmail.com</a>&gt;</p>\n"/* and Joshua Dauth &lt;<a href=\"mailto:kalthar@hotmail.com\">kalthar@hotmail.com</a>&gt;*/
                      "<p>Thanks also to Kito Berg-Taylor for the Qt Ogre intregation code. </p>\n"
                      "<p>Portions Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).  All rights reserved.  Contact: Nokia Corporation (qt-info@nokia.com).  This software contains Qt v.4.7.2.  Qt is licensed under the GNU Lesser General Public License v.2.1, which can be found at http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt. </p>"
                      "<p>Portions Copyright (C) 2000-2011 Torus Knot Software Ltd. OGRE is licensed under the MIT License. "
                      "<p>Uses icons from the Tango Desktop Project &lt;<a href=\"http://tango.freedesktop.org/\">tango.freedesktop.org</a>&gt;</p>\n"
                      "</body></html>"));
    QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(accept()));
    Dialog->setFixedSize(Dialog->sizeHint());
    Dialog->setWindowFlags(Dialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    Dialog->show();
    Dialog->setModal(true);
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

void MainWindow::updateSkybox()
{

}

void MainWindow::resetDefaultSkybox()
{

}

void MainWindow::loadSkyboxImage()
{
    // Get file path
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open texture file"), QString(), tr("Images (*.jpg *.jpeg *.jpe *.png *.tga *.bmp *.raw *.gif *.dds);;JPEG image (*.jpg *.jpeg *.jpe);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp);;RAW image (*.raw);;GIF image (*.gif);;DirectDraw surface (*.dds)"));

    // Return if no file was selected
    if(filePath == 0) return;

    // Convert path to relative
    filePath = QDir::current().relativeFilePath(filePath);

    // Find the appropriate lineEdit box and update it with the new file
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
        ui->skyboxUpLineEdit->setText(filePath);

}
