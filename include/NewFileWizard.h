#ifndef NEWFILEWIZARD_H
#define NEWFILEWIZARD_H

#include <QWizard>
#include "WorldOptions.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE



class NewFileWizard : public QWizard
{
    Q_OBJECT

public:
    NewFileWizard(QWidget* parent = 0);

    void accept();

signals:
    void createdFile(WorldOptions);
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget* parent = 0);

private:
    QLabel* label;
};

class TerrainPage : public QWizardPage
{
    Q_OBJECT

public:
    TerrainPage(QWidget* parent = 0);

private:
    QLabel* label;
};

class TexturesPage : public QWizardPage
{
    Q_OBJECT

public:
    TexturesPage(QWidget* parent = 0);

private:
    QLabel* label;
};

class EnvironmentPage : public QWizardPage
{
    Q_OBJECT

public:
    EnvironmentPage(QWidget* parent = 0);

private:
    QLabel* label;
};

class FoliagePage : public QWizardPage
{
    Q_OBJECT

public:
    FoliagePage(QWidget* parent = 0);

private:
    QLabel* label;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    QLabel *label;
};

#endif // NEWFILEWIZARD_H
