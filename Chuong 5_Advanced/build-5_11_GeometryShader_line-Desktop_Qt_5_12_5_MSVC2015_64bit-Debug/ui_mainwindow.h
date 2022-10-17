/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <axbopenglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionDraw;
    QAction *actionClear;
    QAction *actionWireFrame;
    QWidget *centralwidget;
    AXBOpenGLWidget *openGLWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(473, 350);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget{\n"
"background-color: rgb(68,68,68);\n"
"color: rgb(255,255,255);\n"
"font:10pt;\n"
"}\n"
"QMenuBar{\n"
"background-color: rgb(200,200,200);\n"
"color: rgb(60,60,60);\n"
"}\n"
""));
        actionDraw = new QAction(MainWindow);
        actionDraw->setObjectName(QString::fromUtf8("actionDraw"));
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        actionWireFrame = new QAction(MainWindow);
        actionWireFrame->setObjectName(QString::fromUtf8("actionWireFrame"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        openGLWidget = new AXBOpenGLWidget(centralwidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(100, 20, 300, 200));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 473, 23));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        toolBar->addAction(actionDraw);
        toolBar->addAction(actionClear);
        toolBar->addAction(actionWireFrame);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionDraw->setText(QApplication::translate("MainWindow", "Draw", nullptr));
        actionClear->setText(QApplication::translate("MainWindow", "Clear", nullptr));
        actionWireFrame->setText(QApplication::translate("MainWindow", "WireFrame", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
