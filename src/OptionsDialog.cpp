#include <QMessageBox>
#include "OptionsDialog.h"
#include "ui_OptionsDialog.h"

OptionsDialog::OptionsDialog(QWidget *parent, QSettings* options) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    ui->renderingSubsystemBox->insertItems(0, QStringList()
#ifdef _WIN32
                                       << tr("Direct3D9")
#endif
                                       << tr("OpenGL"));
    setFixedSize(sizeHint());
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    mOptions = options;

    // Set up GUI according to current settings
    ui->invertCameraControlsBox->setChecked(mOptions->value("Renderer/CameraInverted", false).toBool());
    ui->fovSpinBox->setValue(mOptions->value("Renderer/FOV", 45).toInt());
    ui->renderingSubsystemBox->setCurrentIndex(ui->renderingSubsystemBox->findText(mOptions->value("Renderer/RenderingSubsystem").toString()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::accept()
{
    // Tell the user if they have to restert the program to apply settings
    if(mOptions->value("Renderer/RenderingSubsystem") != ui->renderingSubsystemBox->currentText())
        QMessageBox::information(this, "Notice", "Some changes will not be applied until you restart the program", QMessageBox::Ok);

    // Update values acording to new ones entered in the GUI
    mOptions->setValue("Renderer/CameraInverted", ui->invertCameraControlsBox->isChecked());
    mOptions->setValue("Renderer/FOV", ui->fovSpinBox->value());
    mOptions->setValue("Renderer/RenderingSubsystem", ui->renderingSubsystemBox->currentText());

    // Tell the main program to reload settings
    emit settingsUpdated();

    close();
}

void OptionsDialog::closeEvent(QCloseEvent *)
{
    delete this;
}
