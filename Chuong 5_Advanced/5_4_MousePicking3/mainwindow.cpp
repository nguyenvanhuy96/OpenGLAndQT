#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
    connect(ui->openGLWidget, SIGNAL(MousePickingPos(QVector3D) ), this, SLOT(getMousePickingPos(QVector3D)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getMousePickingPos(QVector3D pos)
{
    ui->statusbar->setStyleSheet("font: 14px;");
    ui->statusbar->showMessage("X:"+ QString::number(pos.x(), 'f',2)+
                               "Y:"+ QString::number(pos.y(), 'f',2)+
                               "Z:"+ QString::number(pos.z(), 'f',2));
}

