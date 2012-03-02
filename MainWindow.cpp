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

    // Set mouse cursor
    //mCursorSelect = new QCursor(QPixmap(":/media/cursor-select.png"), 0, 0);
    mOgreWidget->setCursor(QCursor(QPixmap(":/media/cursor-select.png"), 0, 0));
}

MainWindow::~MainWindow()
{
    delete mOgreWidget;
    delete ui;
}
