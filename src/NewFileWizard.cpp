#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSlider>
#include <QSpinBox>
#include "NewFileWizard.h"
#include "algorithms/Random.h"

NewFileWizard::NewFileWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new TerrainPage);
    //addPage(new TexturesPage);
    //addPage(new EnvironmentPage);
    //addPage(new FoliagePage);
    addPage(new ConclusionPage);

    // setPixmaps

    setWindowTitle(tr("New world wizard"));
}

void NewFileWizard::accept()
{
    WorldOptions worldOptions;

    worldOptions.terrainSeed = Random::getSingleton().getRand(0, 2147483647);
    worldOptions.terrainRandomFactor = field("randomFactor").toInt();
    worldOptions.terrainScale = field("terrainScale").toInt();
    worldOptions.erosionPasses = field("erosionPasses").toInt();
    worldOptions.terrainSize = field("terrainSize").toDouble();
    /*worldOptions.layers[0].diffuseSpecular = field("layer0diffuseSpecular").toString();
    worldOptions.layers[0].normalHeight = field("layer0normalHeight").toString();
    worldOptions.layers[0].placementHeight = field("layer0placementHeight").toInt();
    worldOptions.layers[0].scale = field("layer0scale").toInt();
    worldOptions.layers[1].diffuseSpecular = field("layer1diffuseSpecular").toString();
    worldOptions.layers[1].normalHeight = field("layer1normalHeight").toString();
    worldOptions.layers[1].placementHeight = field("layer1placementHeight").toInt();
    worldOptions.layers[1].scale = field("layer1scale").toInt();
    worldOptions.layers[2].diffuseSpecular = field("layer2diffuseSpecular").toString();
    worldOptions.layers[2].normalHeight = field("layer2normalHeight").toString();
    worldOptions.layers[2].placementHeight = field("layer2placementHeight").toInt();
    worldOptions.layers[2].scale = field("layer2scale").toInt();
    worldOptions.skyDome = field("skyDome").toString();
    worldOptions.skyCurvature = field("skyCurvature").toDouble();
    worldOptions.skyTiling = field("skyTiling").toInt();*/

    emit createdFile(worldOptions);

    QDialog::accept();
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));

    label = new QLabel(tr("This wizard will generate a new, completely random "
                          "terrain, allowing you to specify parameters to "
                          "control the generation process. Everything created "
                          "with this wizard can be modified after the world has "
                          "been generated and it is also possible to manually "
                          "modify the terrain to suit your needs. "));

    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

TerrainPage::TerrainPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Terrain"));
    setSubTitle(tr("Specify parameters to control the shape of the terrain to be generated"));

    // Create GUI
    QFormLayout* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    QLabel* label_0 = new QLabel(tr("Random factor"));
    formLayout->setWidget(0, QFormLayout::LabelRole, label_0);

    QSlider* randomFactorSlider = new QSlider(this);
    randomFactorSlider->setMaximum(510);
    randomFactorSlider->setValue(255);
    randomFactorSlider->setOrientation(Qt::Horizontal);
    formLayout->setWidget(0, QFormLayout::FieldRole, randomFactorSlider);

    QLabel* label_1 = new QLabel(tr("Scale"));
    formLayout->setWidget(1, QFormLayout::LabelRole, label_1);

    QSlider* terrainScaleSlider = new QSlider(this);
    terrainScaleSlider->setMaximum(3600);
    terrainScaleSlider->setValue(1800);
    terrainScaleSlider->setOrientation(Qt::Horizontal);
    formLayout->setWidget(1, QFormLayout::FieldRole, terrainScaleSlider);

    QLabel* label_2 = new QLabel(tr("Erosion"));
    formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

    QSlider* erosionSlider = new QSlider(this);
    erosionSlider->setMaximum(30);
    erosionSlider->setValue(1);
    erosionSlider->setOrientation(Qt::Horizontal);
    formLayout->setWidget(2, QFormLayout::FieldRole, erosionSlider);

    QLabel* label_3 = new QLabel(tr("Terrain size"));
    formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

    QDoubleSpinBox* terrainSizeSpinBox = new QDoubleSpinBox(this);
    terrainSizeSpinBox->setMaximum(100000);
    terrainSizeSpinBox->setValue(5000);
    formLayout->setWidget(3, QFormLayout::FieldRole, terrainSizeSpinBox);

    registerField("randomFactor", randomFactorSlider);
    registerField("terrainScale", terrainScaleSlider);
    registerField("erosionPasses", erosionSlider);
    registerField("terrainSize", terrainSizeSpinBox);

    setLayout(formLayout);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the world.")
                   .arg(finishText));
}
