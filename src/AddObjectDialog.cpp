#include "AddObjectDialog.h"
#include "ui_AddObjectDialog.h"
#include <QFileDialog>
#include <QDir>

AddObjectDialog::AddObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddObjectDialog)
{
    ui->setupUi(this);
}

AddObjectDialog::~AddObjectDialog()
{
    delete ui;
}

void AddObjectDialog::accept()
{
    emit textureSelected(ui->filePath->text());
}

void AddObjectDialog::loadTexture()
{
    // Get file path
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open texture file"), QString(), tr("Images (*.jpg *.jpeg *.jpe *.png *.tga *.bmp *.raw *.gif *.dds);;JPEG image (*.jpg *.jpeg *.jpe);;PNG image (*.png);;Targa image (*.tga);;Bitmap image (*.bmp);;RAW image (*.raw);;GIF image (*.gif);;DirectDraw surface (*.dds)"));

    // Return if no file was selected
    if(filePath == 0) return;

    // Convert path to relative
    filePath = QDir::current().relativeFilePath(filePath);

    ui->filePath->setText(filePath);
}
