#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QFileDialog>
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

    ui->setupUi(this);
    mOgreWidget = new OgreWidget;
    setCentralWidget(mOgreWidget);
    mOgreWidget->setInteractionMode(OgreWidget::IM_SELECT);

    // tabify dock widgets
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::East);
    tabifyDockWidget(ui->terrainDockWidget, ui->texturesDockWidget);
    tabifyDockWidget(ui->texturesDockWidget, ui->skyDockWidget);
    tabifyDockWidget(ui->skyDockWidget, ui->foliageDockWidget);
    ui->terrainDockWidget->raise();

    // Set up action group for tool buttons
    mToolGroup = new QActionGroup(this);
    mToolGroup->addAction(ui->actionSelect);
    mToolGroup->addAction(ui->actionPlace_entities);
    mToolGroup->addAction(ui->actionPaint);
    mToolGroup->addAction(ui->actionExtrude);
    mToolGroup->addAction(ui->actionIntrude);

    // set up timer to get the Ogre widget to render at 60fps
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), mOgreWidget, SLOT(updateGL()));
    timer->start(1000.0f/60.0f);
}

MainWindow::~MainWindow()
{
    delete mOgreWidget;
    delete mRenderTimer;
    delete mToolGroup;
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

void MainWindow::viewTextured()
{

}

void MainWindow::viewWireframe()
{
    mOgreWidget->setViewMode(Ogre::PM_WIREFRAME);
}

void MainWindow::screenshot()
{
    mOgreWidget->saveScreenshotToFile(QFileDialog::getSaveFileName(this, tr("Save Image"),
                                             "", tr("JPEG Image (*.jpg *.jpeg);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp)")));
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
