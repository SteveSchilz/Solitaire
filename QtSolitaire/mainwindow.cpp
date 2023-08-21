#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "card.h"
#include "Constants.h"
#include "myScene.h"

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mScene(nullptr)
{
    ui->setupUi(this);

    qDebug() << "Hello";
    mScene = new  myScene(-GAME_WIDTH/2, -GAME_HEIGHT/2, GAME_WIDTH, GAME_HEIGHT, ui->centralwidget);
    mScene->setSceneRect(QRectF(-GAME_WIDTH/2, -GAME_HEIGHT/2, GAME_WIDTH, GAME_HEIGHT));
    for (Suite suite: SuiteIterator() ) {
        for (CardValue value: CardValueIterator()) {

            Card *item = new Card(value, suite, nullptr);
                item->setPos(30-(GAME_WIDTH/2)+(double)(value)*30.0, 100-(GAME_HEIGHT/2)+(double)suite*22);

            mScene->addItem(item);
        }
    }

//    QGraphicsView* view = ui->centralwidget->findChild<QGraphicsView*>("graphicsView");
    QGraphicsView *view = new QGraphicsView{ui->centralwidget};
    if (view) {
        view->setScene(mScene);
        view->setRenderHint(QPainter::Antialiasing);
        view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        view->setBackgroundBrush(QColor(22, 161, 39));      // A medium dark green
        view->show();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

