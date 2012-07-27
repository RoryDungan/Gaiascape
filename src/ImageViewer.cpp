#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "ImageViewer.h"
#include "ui_ImageViewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    mImageLabel = new QLabel;
    mImageLabel->setBackgroundRole(QPalette::Base);
    mImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mImageLabel->setScaledContents(true);

    ui->setupUi(this);
    // A few fixes for things which can't be defined in the .ui file
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(mImageLabel);

    // Set up action group for tool buttons
    QActionGroup* mToolGroup = new QActionGroup(this);
    mToolGroup->addAction(ui->actionPan);
    mToolGroup->addAction(ui->actionExtrude);
    mToolGroup->addAction(ui->actionIntrude);

    ui->scrollArea->setWidgetResizable(false);

    // Init variables
    currentState = IS_IDLE;
}

ImageViewer::~ImageViewer()
{
    if(mImageLabel) delete mImageLabel;
    delete ui;
}

void ImageViewer::loadImage(QPixmap image)
{
    ui->toolBar->setEnabled(true);

    // Show image
    mImageLabel->setPixmap(image);

    // Scale image to fit the window
    dScaleFactor = 514 / mImageLabel->pixmap()->size().height();

    mImageLabel->resize(514, 514);
}

void ImageViewer::mousePressEvent(QMouseEvent * event)
{
    // Update relative cursor position to avoid "jumping" effect
    mLastCursorPos = event->pos();

    if(event->button() == Qt::MiddleButton)
        currentState = IS_PANNING;
}

void ImageViewer::mouseReleaseEvent(QMouseEvent * event)
{
    currentState = IS_IDLE;
}

void ImageViewer::mouseMoveEvent(QMouseEvent * event)
{
    // Get amount of movement instead of absolute position
    int dx = event->x() - mLastCursorPos.x();
    int dy = event->y() - mLastCursorPos.y();

    switch(currentState)
    {
    case IS_IDLE:
        break;
    case IS_PANNING:
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value() - dx);
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - dy);
        break;
    case IS_PAINTING:
        break;
    default:
        break;
    }

    mLastCursorPos = event->pos();
}

void ImageViewer::saveAs()
{
    // Diaog for the user to choose where to save the fle
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save heightmap as image"), QString(), tr("PNG image (*.png);;JPEG Image (*.jpg *.jpeg);;Windows Bitmap (*.bmp);;Portable Pixmap (*.ppm);;Tagged Image File Format (*.tiff)"));
    // Cancel if there was no path entered
    if(filepath ==0) return;

   mImageLabel->pixmap()->toImage().save(filepath);
}

void ImageViewer::showProperties()
{
    // stub
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::resetZoom()
{
    mImageLabel->adjustSize();
    dScaleFactor = 1.0;
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(mImageLabel->pixmap());
    dScaleFactor *= factor;
    mImageLabel->resize(dScaleFactor * mImageLabel->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    ui->actionZoom_in->setEnabled(dScaleFactor < 10.0);
    ui->actionZoom_out->setEnabled(dScaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                        + ((factor - 1) * scrollBar->pageStep()/2)));
}

