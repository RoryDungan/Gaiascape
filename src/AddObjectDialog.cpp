#include "AddObjectDialog.h"
#include "ui_AddObjectDialog.h"

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

