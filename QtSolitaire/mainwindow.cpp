#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Hello";

    // Game is automatically shown by the framework.
    mGame = new Game{ui->centralwidget, ui->menubar};

}

MainWindow::~MainWindow()
{
    delete ui;
}

