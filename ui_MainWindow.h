/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon 27. Feb 00:31:29 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
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
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Edit;
    QMenu *menu_View;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 408);
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/media/24x24/document-new.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_New->setIcon(icon);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/media/24x24/document-open.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon1);
        actionOpen_Recent = new QAction(MainWindow);
        actionOpen_Recent->setObjectName(QString::fromUtf8("actionOpen_Recent"));
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/media/24x24/document-save-as.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon2);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QString::fromUtf8("actionE_xit"));
        action_Undo = new QAction(MainWindow);
        action_Undo->setObjectName(QString::fromUtf8("action_Undo"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/media/24x24/edit-undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Undo->setIcon(icon3);
        action_Redo = new QAction(MainWindow);
        action_Redo->setObjectName(QString::fromUtf8("action_Redo"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/media/24x24/edit-redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Redo->setIcon(icon4);
        action_Textured = new QAction(MainWindow);
        action_Textured->setObjectName(QString::fromUtf8("action_Textured"));
        action_Solid = new QAction(MainWindow);
        action_Solid->setObjectName(QString::fromUtf8("action_Solid"));
        action_Wireframe = new QAction(MainWindow);
        action_Wireframe->setObjectName(QString::fromUtf8("action_Wireframe"));
        actionExtrude = new QAction(MainWindow);
        actionExtrude->setObjectName(QString::fromUtf8("actionExtrude"));
        actionExtrude->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/media/24x24/go-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExtrude->setIcon(icon5);
        actionIntrude = new QAction(MainWindow);
        actionIntrude->setObjectName(QString::fromUtf8("actionIntrude"));
        actionIntrude->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/media/24x24/go-down.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionIntrude->setIcon(icon6);
        actionPaint = new QAction(MainWindow);
        actionPaint->setObjectName(QString::fromUtf8("actionPaint"));
        actionPaint->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/media/24x24/paint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaint->setIcon(icon7);
        actionSelect = new QAction(MainWindow);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        actionSelect->setCheckable(true);
        actionPlace_entities = new QAction(MainWindow);
        actionPlace_entities->setObjectName(QString::fromUtf8("actionPlace_entities"));
        actionPlace_entities->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/media/24x24/list-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlace_entities->setIcon(icon8);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QString::fromUtf8("menu_Edit"));
        menu_View = new QMenu(menuBar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuBar->addAction(menu_View->menuAction());
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
        menu_View->addAction(action_Textured);
        menu_View->addAction(action_Solid);
        menu_View->addAction(action_Wireframe);
        mainToolBar->addAction(action_New);
        mainToolBar->addAction(action_Open);
        mainToolBar->addAction(action_Save);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Undo);
        mainToolBar->addAction(action_Redo);
        toolBar->addAction(actionSelect);
        toolBar->addAction(actionPlace_entities);
        toolBar->addAction(actionPaint);
        toolBar->addAction(actionExtrude);
        toolBar->addAction(actionIntrude);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
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
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        menu_Edit->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
