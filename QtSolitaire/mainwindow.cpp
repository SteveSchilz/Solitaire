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
    mScene = new  myScene(-200, -200, GAME_WIDTH, GAME_HEIGHT, ui->centralwidget);
    CardValue value;
    Suite suite = Suite::DIAMOND;
    for (int i = 0; i < 13; i++) {
        value = static_cast <CardValue>(i+1);
        if (i >= 6) {
            suite = Suite::CLUB;
        }
        Card *item = new Card(value, suite, nullptr);
        switch(value) {
        case CardValue::KING: item->setImage(":/images/King-Hearts.svg"); break;
        case CardValue::QUEEN: item->setImage(":/images/Queen-Hearts.svg"); break;
        case CardValue::JACK: item->setImage(":/images/Jack-Hearts.svg"); break;
        default:    break;
        }

        item->setPos(::sin((value * 6.28) / 14.0) * 150,
                     ::cos((value * 6.28) / 14.0) * 150);

        mScene->addItem(item);
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

