#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QScrollBar>
#include <QLabel>
#include <QMouseEvent>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ImageViewer(QWidget *parent = 0, QString imageFile="");
    ~ImageViewer();

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    
private:
    Ui::ImageViewer *ui;

    double dScaleFactor; // Factor for scaling the image
    QLabel* mImageLabel;

    enum interactionState { IS_IDLE, IS_PANNING, IS_PAINTING } currentState;
    QPoint mLastCursorPos;

    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

public slots:
    void saveAs();
    void showProperties();
    void zoomIn();
    void zoomOut();
    void resetZoom();
};

#endif // IMAGEVIEWER_H
