#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mOgreWidget = new OgreWidget;
    setCentralWidget(mOgreWidget);

    // tabify dock widgets
    tabifyDockWidget(ui->terrainDockWidget, ui->texturesDockWidget);
    tabifyDockWidget(ui->texturesDockWidget, ui->skyDockWidget);
    ui->terrainDockWidget->raise();

    // Set up action group for tool buttons
    mToolGroup = new QActionGroup(this);
    mToolGroup->addAction(ui->actionSelect);
    mToolGroup->addAction(ui->actionPlace_entities);
    mToolGroup->addAction(ui->actionPaint);
    mToolGroup->addAction(ui->actionExtrude);
    mToolGroup->addAction(ui->actionIntrude);
}

MainWindow::~MainWindow()
{
    delete mOgreWidget;
    delete ui;
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

}

void MainWindow::addEntTool()
{

}

void MainWindow::paintTool()
{

}

void MainWindow::extrudeTool()
{

}

void MainWindow::intrudeTool()
{

}
