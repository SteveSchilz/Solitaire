#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "card.h"
#include "cardstack.h"
#include "constants.h"
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
    , mScene{nullptr}
    , mHearts{nullptr}
    , mDiamonds{nullptr}
    , mClubs{nullptr}
    , mSpades{nullptr}
{
    ui->setupUi(this);

    qDebug() << "Hello";
    mScene = new  myScene(0, 0, GAME_WIDTH, GAME_HEIGHT, ui->centralwidget);
    mScene->setSceneRect(QRectF(0, 0, GAME_WIDTH, GAME_HEIGHT));
    for (Suite suite: SuiteIterator() ) {
        for (CardValue value: CardValueIterator()) {

            Card *item = new Card(value, suite, nullptr);
                item->setPos(30+(double)(value)*30.0, 3*CARD_HEIGHT/2+2*TOP_MARGIN+(double)suite*22);

            mScene->addItem(item);
        }
    }
    for (Suite suite: SuiteIterator()) {
        SortedStack *stack = new SortedStack(suite, nullptr);
        stack->setPos(GAME_WIDTH/2 + (double)suite*(CARD_WIDTH+CARD_SPACING), TOP_MARGIN+CARD_HEIGHT/2);
        stack->setTransform(QTransform::fromScale(1.0, 1.0), true);
        mScene->addItem(stack);
        switch(suite) {
        case Suite::HEART: mHearts = stack; break;
        case Suite::DIAMOND: mDiamonds = stack; break;
        case Suite::CLUB:   mClubs = stack; break;
        case Suite::SPADE:  mSpades = stack; break;
        }
    }

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

