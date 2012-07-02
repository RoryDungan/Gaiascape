/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon 2. Jul 15:21:40 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCommandLinkButton>
#include <QtGui/QDockWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New;
    QAction *action_Open;
    QAction *actionOpen_Recent;
    QAction *action_Save;
    QAction *actionSave_As;
    QAction *actionE_xit;
    QAction *action_Undo;
    QAction *action_Redo;
    QAction *action_Textured;
    QAction *action_Solid;
    QAction *action_Wireframe;
    QAction *actionExtrude;
    QAction *actionIntrude;
    QAction *actionPaint;
    QAction *actionSelect;
    QAction *actionPlace_entities;
    QAction *actionTake_screenshot;
    QAction *actionShow_heightmap_image;
    QAction *actionUser_guide;
    QAction *actionAbout_Gaiascape;
    QAction *actionOptions;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Edit;
    QMenu *menu_View;
    QMenu *menuTools;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QDockWidget *texturesDockWidget;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout;
    QTreeWidget *texturesTreeWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *updateTexturesButton;
    QPushButton *resetTexturesButton;
    QDockWidget *skyDockWidget;
    QWidget *dockWidgetContents_3;
    QDockWidget *foliageDockWidget;
    QWidget *dockWidgetContents_4;
    QDockWidget *terrainDockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QFormLayout *formLayout_3;
    QLabel *label_2;
    QSlider *terrainScaleSlider;
    QLabel *label_3;
    QSlider *ramdomFactorSlider;
    QLabel *label_4;
    QSpinBox *spinBox;
    QLabel *label_5;
    QSlider *horizontalSlider;
    QWidget *tab_2;
    QFormLayout *formLayout_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpinBox *randomSeedBox;
    QToolButton *randomiseToolButton;
    QCommandLinkButton *generateTerrain;
    QCommandLinkButton *loadTerrain;
    QCommandLinkButton *clearTerrain;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(868, 680);
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        action_New->setEnabled(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/media/24x24/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_New->setIcon(icon);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        action_Open->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/media/24x24/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon1);
        actionOpen_Recent = new QAction(MainWindow);
        actionOpen_Recent->setObjectName(QString::fromUtf8("actionOpen_Recent"));
        actionOpen_Recent->setEnabled(false);
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        action_Save->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/media/24x24/document-save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon2);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionSave_As->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/media/16x16/document-save-as.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon3);
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/media/16x16/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionE_xit->setIcon(icon4);
        action_Undo = new QAction(MainWindow);
        action_Undo->setObjectName(QString::fromUtf8("action_Undo"));
        action_Undo->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/media/24x24/edit-undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Undo->setIcon(icon5);
        action_Redo = new QAction(MainWindow);
        action_Redo->setObjectName(QString::fromUtf8("action_Redo"));
        action_Redo->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/media/24x24/edit-redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Redo->setIcon(icon6);
        action_Textured = new QAction(MainWindow);
        action_Textured->setObjectName(QString::fromUtf8("action_Textured"));
        action_Solid = new QAction(MainWindow);
        action_Solid->setObjectName(QString::fromUtf8("action_Solid"));
        action_Wireframe = new QAction(MainWindow);
        action_Wireframe->setObjectName(QString::fromUtf8("action_Wireframe"));
        actionExtrude = new QAction(MainWindow);
        actionExtrude->setObjectName(QString::fromUtf8("actionExtrude"));
        actionExtrude->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/media/24x24/go-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExtrude->setIcon(icon7);
        actionIntrude = new QAction(MainWindow);
        actionIntrude->setObjectName(QString::fromUtf8("actionIntrude"));
        actionIntrude->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/media/24x24/go-down.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionIntrude->setIcon(icon8);
        actionPaint = new QAction(MainWindow);
        actionPaint->setObjectName(QString::fromUtf8("actionPaint"));
        actionPaint->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/media/24x24/paint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaint->setIcon(icon9);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        actionSelect->setCheckable(true);
        actionSelect->setChecked(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/media/24x24/cursor-select.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect->setIcon(icon10);
        actionPlace_entities = new QAction(MainWindow);
        actionPlace_entities->setObjectName(QString::fromUtf8("actionPlace_entities"));
        actionPlace_entities->setCheckable(true);
        actionPlace_entities->setEnabled(false);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/media/24x24/list-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlace_entities->setIcon(icon11);
        actionTake_screenshot = new QAction(MainWindow);
        actionTake_screenshot->setObjectName(QString::fromUtf8("actionTake_screenshot"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/media/24x24/camera-photo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTake_screenshot->setIcon(icon12);
        actionShow_heightmap_image = new QAction(MainWindow);
        actionShow_heightmap_image->setObjectName(QString::fromUtf8("actionShow_heightmap_image"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/media/16x16/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShow_heightmap_image->setIcon(icon13);
        actionUser_guide = new QAction(MainWindow);
        actionUser_guide->setObjectName(QString::fromUtf8("actionUser_guide"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/media/16x16/help-browser.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUser_guide->setIcon(icon14);
        actionAbout_Gaiascape = new QAction(MainWindow);
        actionAbout_Gaiascape->setObjectName(QString::fromUtf8("actionAbout_Gaiascape"));
        actionOptions = new QAction(MainWindow);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/media/16x16/preferences-system.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOptions->setIcon(icon15);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 868, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        menu_View = new QMenu(menuBar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setIconSize(QSize(24, 24));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);
        texturesDockWidget = new QDockWidget(MainWindow);
        texturesDockWidget->setObjectName(QString::fromUtf8("texturesDockWidget"));
        texturesDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        texturesTreeWidget = new QTreeWidget(dockWidgetContents_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(texturesTreeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(texturesTreeWidget);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        texturesTreeWidget->setObjectName(QString::fromUtf8("texturesTreeWidget"));
        texturesTreeWidget->setAlternatingRowColors(true);
        texturesTreeWidget->header()->setDefaultSectionSize(100);
        texturesTreeWidget->header()->setHighlightSections(true);

        verticalLayout->addWidget(texturesTreeWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        updateTexturesButton = new QPushButton(dockWidgetContents_2);
        updateTexturesButton->setObjectName(QString::fromUtf8("updateTexturesButton"));

        horizontalLayout_2->addWidget(updateTexturesButton);

        resetTexturesButton = new QPushButton(dockWidgetContents_2);
        resetTexturesButton->setObjectName(QString::fromUtf8("resetTexturesButton"));

        horizontalLayout_2->addWidget(resetTexturesButton);


        verticalLayout->addLayout(horizontalLayout_2);

        texturesDockWidget->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), texturesDockWidget);
        skyDockWidget = new QDockWidget(MainWindow);
        skyDockWidget->setObjectName(QString::fromUtf8("skyDockWidget"));
        skyDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        skyDockWidget->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), skyDockWidget);
        foliageDockWidget = new QDockWidget(MainWindow);
        foliageDockWidget->setObjectName(QString::fromUtf8("foliageDockWidget"));
        foliageDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
        foliageDockWidget->setWidget(dockWidgetContents_4);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), foliageDockWidget);
        terrainDockWidget = new QDockWidget(MainWindow);
        terrainDockWidget->setObjectName(QString::fromUtf8("terrainDockWidget"));
        terrainDockWidget->setMinimumSize(QSize(263, 305));
        terrainDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget = new QTabWidget(dockWidgetContents);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        formLayout_3 = new QFormLayout(tab);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_2);

        terrainScaleSlider = new QSlider(tab);
        terrainScaleSlider->setObjectName(QString::fromUtf8("terrainScaleSlider"));
        terrainScaleSlider->setMaximum(100);
        terrainScaleSlider->setValue(50);
        terrainScaleSlider->setOrientation(Qt::Horizontal);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, terrainScaleSlider);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_3);

        ramdomFactorSlider = new QSlider(tab);
        ramdomFactorSlider->setObjectName(QString::fromUtf8("ramdomFactorSlider"));
        ramdomFactorSlider->setMaximum(100);
        ramdomFactorSlider->setValue(50);
        ramdomFactorSlider->setOrientation(Qt::Horizontal);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, ramdomFactorSlider);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_4);

        spinBox = new QSpinBox(tab);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(2);
        spinBox->setMaximum(98);
        spinBox->setSingleStep(2);
        spinBox->setValue(8);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, spinBox);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_5);

        horizontalSlider = new QSlider(tab);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setSliderPosition(50);
        horizontalSlider->setOrientation(Qt::Horizontal);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, horizontalSlider);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        formLayout_2 = new QFormLayout(tab_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label = new QLabel(tab_2);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        randomSeedBox = new QSpinBox(tab_2);
        randomSeedBox->setObjectName(QString::fromUtf8("randomSeedBox"));
        randomSeedBox->setMaximum(2147483647);
        randomSeedBox->setValue(1);

        horizontalLayout->addWidget(randomSeedBox);

        randomiseToolButton = new QToolButton(tab_2);
        randomiseToolButton->setObjectName(QString::fromUtf8("randomiseToolButton"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/media/16x16/dice-random.png"), QSize(), QIcon::Normal, QIcon::Off);
        randomiseToolButton->setIcon(icon16);

        horizontalLayout->addWidget(randomiseToolButton);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout);

        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        generateTerrain = new QCommandLinkButton(dockWidgetContents);
        generateTerrain->setObjectName(QString::fromUtf8("generateTerrain"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/media/22x22/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        generateTerrain->setIcon(icon17);
        generateTerrain->setIconSize(QSize(22, 22));

        verticalLayout_2->addWidget(generateTerrain);

        loadTerrain = new QCommandLinkButton(dockWidgetContents);
        loadTerrain->setObjectName(QString::fromUtf8("loadTerrain"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/media/22x22/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        loadTerrain->setIcon(icon18);
        loadTerrain->setIconSize(QSize(22, 22));

        verticalLayout_2->addWidget(loadTerrain);

        clearTerrain = new QCommandLinkButton(dockWidgetContents);
        clearTerrain->setObjectName(QString::fromUtf8("clearTerrain"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/media/22x22/process-stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearTerrain->setIcon(icon19);
        clearTerrain->setIconSize(QSize(22, 22));

        verticalLayout_2->addWidget(clearTerrain);

        terrainDockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), terrainDockWidget);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuBar->addAction(menu_View->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu_File->addAction(action_New);
        menu_File->addAction(action_Open);
        menu_File->addAction(actionOpen_Recent);
        menu_File->addSeparator();
        menu_File->addAction(action_Save);
        menu_File->addAction(actionSave_As);
        menu_File->addSeparator();
        menu_File->addAction(actionE_xit);
        menu_Edit->addAction(action_Undo);
        menu_Edit->addAction(action_Redo);
        menu_View->addAction(actionShow_heightmap_image);
        menu_View->addSeparator();
        menu_View->addAction(action_Solid);
        menu_View->addAction(action_Wireframe);
        menuTools->addAction(actionTake_screenshot);
        menuTools->addSeparator();
        menuTools->addAction(actionOptions);
        menuHelp->addAction(actionUser_guide);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout_Gaiascape);
        mainToolBar->addAction(action_New);
        mainToolBar->addAction(action_Open);
        mainToolBar->addAction(action_Save);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Undo);
        mainToolBar->addAction(action_Redo);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionTake_screenshot);
        toolBar->addAction(actionSelect);
        toolBar->addAction(actionPlace_entities);
        toolBar->addAction(actionPaint);
        toolBar->addAction(actionExtrude);
        toolBar->addAction(actionIntrude);

        retranslateUi(MainWindow);
        QObject::connect(actionSelect, SIGNAL(triggered()), MainWindow, SLOT(selectTool()));
        QObject::connect(actionE_xit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(action_New, SIGNAL(triggered()), MainWindow, SLOT(fileNew()));
        QObject::connect(action_Open, SIGNAL(triggered()), MainWindow, SLOT(fileOpen()));
        QObject::connect(action_Save, SIGNAL(triggered()), MainWindow, SLOT(fileSave()));
        QObject::connect(actionSave_As, SIGNAL(triggered()), MainWindow, SLOT(fileSaveAs()));
        QObject::connect(action_Undo, SIGNAL(triggered()), MainWindow, SLOT(editUndo()));
        QObject::connect(action_Redo, SIGNAL(triggered()), MainWindow, SLOT(editRedo()));
        QObject::connect(actionExtrude, SIGNAL(triggered()), MainWindow, SLOT(extrudeTool()));
        QObject::connect(actionIntrude, SIGNAL(triggered()), MainWindow, SLOT(intrudeTool()));
        QObject::connect(actionPaint, SIGNAL(triggered()), MainWindow, SLOT(paintTool()));
        QObject::connect(actionPlace_entities, SIGNAL(triggered()), MainWindow, SLOT(addEntTool()));
        QObject::connect(action_Wireframe, SIGNAL(triggered()), MainWindow, SLOT(viewWireframe()));
        QObject::connect(action_Solid, SIGNAL(triggered()), MainWindow, SLOT(viewSolid()));
        QObject::connect(actionTake_screenshot, SIGNAL(triggered()), MainWindow, SLOT(screenshot()));
        QObject::connect(texturesTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), MainWindow, SLOT(texturePropertyChanged(QTreeWidgetItem*,int)));
        QObject::connect(actionShow_heightmap_image, SIGNAL(triggered()), MainWindow, SLOT(showHeightmapImage()));
        QObject::connect(generateTerrain, SIGNAL(clicked()), MainWindow, SLOT(generateTerrain()));
        QObject::connect(loadTerrain, SIGNAL(clicked()), MainWindow, SLOT(loadTerrain()));
        QObject::connect(clearTerrain, SIGNAL(clicked()), MainWindow, SLOT(clearTerrain()));
        QObject::connect(randomiseToolButton, SIGNAL(clicked()), MainWindow, SLOT(randomiseTerrainSeed()));
        QObject::connect(actionOptions, SIGNAL(triggered()), MainWindow, SLOT(options()));
        QObject::connect(actionAbout_Gaiascape, SIGNAL(triggered()), MainWindow, SLOT(showAboutBox()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Gaiascape", 0, QApplication::UnicodeUTF8));
        action_New->setText(QApplication::translate("MainWindow", "&New...", 0, QApplication::UnicodeUTF8));
        action_New->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        action_Open->setText(QApplication::translate("MainWindow", "&Open...", 0, QApplication::UnicodeUTF8));
        action_Open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionOpen_Recent->setText(QApplication::translate("MainWindow", "Open &Recent", 0, QApplication::UnicodeUTF8));
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        action_Save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save &As...", 0, QApplication::UnicodeUTF8));
        actionSave_As->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        actionE_xit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        action_Undo->setText(QApplication::translate("MainWindow", "&Undo", 0, QApplication::UnicodeUTF8));
        action_Undo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        action_Redo->setText(QApplication::translate("MainWindow", "&Redo", 0, QApplication::UnicodeUTF8));
        action_Redo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        action_Textured->setText(QApplication::translate("MainWindow", "&Textured", 0, QApplication::UnicodeUTF8));
        action_Solid->setText(QApplication::translate("MainWindow", "&Solid", 0, QApplication::UnicodeUTF8));
        action_Wireframe->setText(QApplication::translate("MainWindow", "&Wireframe", 0, QApplication::UnicodeUTF8));
        actionExtrude->setText(QApplication::translate("MainWindow", "Extrude", 0, QApplication::UnicodeUTF8));
        actionIntrude->setText(QApplication::translate("MainWindow", "Intrude", 0, QApplication::UnicodeUTF8));
        actionPaint->setText(QApplication::translate("MainWindow", "Paint", 0, QApplication::UnicodeUTF8));
        actionSelect->setText(QApplication::translate("MainWindow", "Select", 0, QApplication::UnicodeUTF8));
        actionPlace_entities->setText(QApplication::translate("MainWindow", "Place entities", 0, QApplication::UnicodeUTF8));
        actionTake_screenshot->setText(QApplication::translate("MainWindow", "&Save screenshot...", 0, QApplication::UnicodeUTF8));
        actionTake_screenshot->setShortcut(QApplication::translate("MainWindow", "F12", 0, QApplication::UnicodeUTF8));
        actionShow_heightmap_image->setText(QApplication::translate("MainWindow", "Show heightmap image", 0, QApplication::UnicodeUTF8));
        actionUser_guide->setText(QApplication::translate("MainWindow", "User guide", 0, QApplication::UnicodeUTF8));
        actionUser_guide->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        actionAbout_Gaiascape->setText(QApplication::translate("MainWindow", "About Gaiascape", 0, QApplication::UnicodeUTF8));
        actionOptions->setText(QApplication::translate("MainWindow", "&Options...", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow", "Home", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        texturesDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Textures", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = texturesTreeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MainWindow", "Value", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "Property", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = texturesTreeWidget->isSortingEnabled();
        texturesTreeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = texturesTreeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "Grass", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(1, QApplication::translate("MainWindow", "growth_weirdfungus-03_diffusespecular.dds", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "Diffuse", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem1->child(1);
        ___qtreewidgetitem3->setText(1, QApplication::translate("MainWindow", "growth_weirdfungus-03_normalheight.dds", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindow", "Normal map", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem1->child(2);
        ___qtreewidgetitem4->setText(1, QApplication::translate("MainWindow", "200", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem4->setText(0, QApplication::translate("MainWindow", "Placement height", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem5 = texturesTreeWidget->topLevelItem(1);
        ___qtreewidgetitem5->setText(0, QApplication::translate("MainWindow", "Dirt", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem5->child(0);
        ___qtreewidgetitem6->setText(1, QApplication::translate("MainWindow", "dirt_grayrocky_diffusespecular.dds", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem6->setText(0, QApplication::translate("MainWindow", "Diffuse", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem5->child(1);
        ___qtreewidgetitem7->setText(1, QApplication::translate("MainWindow", "dirt_grayrocky_normalheight.dds", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem7->setText(0, QApplication::translate("MainWindow", "Normal map", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem5->child(2);
        ___qtreewidgetitem8->setText(1, QApplication::translate("MainWindow", "100", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem8->setText(0, QApplication::translate("MainWindow", "Placement height", 0, QApplication::UnicodeUTF8));
        texturesTreeWidget->setSortingEnabled(__sortingEnabled);

        updateTexturesButton->setText(QApplication::translate("MainWindow", "Update terrain", 0, QApplication::UnicodeUTF8));
        resetTexturesButton->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        skyDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Sky", 0, QApplication::UnicodeUTF8));
        foliageDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Foliage", 0, QApplication::UnicodeUTF8));
        terrainDockWidget->setWindowTitle(QApplication::translate("MainWindow", "Terrain", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Terrain scale", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Random factor", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Terrain size", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Erroson", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Basic terrain options", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Random seed", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        randomiseToolButton->setToolTip(QApplication::translate("MainWindow", "Randomise", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        randomiseToolButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Advanced", 0, QApplication::UnicodeUTF8));
        generateTerrain->setText(QApplication::translate("MainWindow", "Generate new terrain", 0, QApplication::UnicodeUTF8));
        generateTerrain->setDescription(QApplication::translate("MainWindow", "Create a terrain according to the specified parameters", 0, QApplication::UnicodeUTF8));
        loadTerrain->setText(QApplication::translate("MainWindow", "Import terrain from file", 0, QApplication::UnicodeUTF8));
        loadTerrain->setDescription(QApplication::translate("MainWindow", "Create a terrain from an existing file", 0, QApplication::UnicodeUTF8));
        clearTerrain->setText(QApplication::translate("MainWindow", "Clear terrain", 0, QApplication::UnicodeUTF8));
        clearTerrain->setDescription(QApplication::translate("MainWindow", "Flatten the entire terrain", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
