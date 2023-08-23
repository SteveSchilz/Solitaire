#include "game.h"

#include "card.h"
#include "cardstack.h"
#include "clickableitem.h"
#include "constants.h"
#include "deck.h"
#include "myScene.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <QMessageBox>

const bool showDeck{true};  //< Debug flag to show initial state of deck.

Game::Game(QWidget* parent)
    : QGraphicsView{parent}
    , mScene{nullptr}
    , mDeck{nullptr}
    , mHand{nullptr}
    , mWastePile{nullptr}
    , mHearts{nullptr}
    , mDiamonds{nullptr}
    , mClubs{nullptr}
    , mSpades{nullptr}

{
    mScene = new  myScene(0, 0, GAME_WIDTH, GAME_HEIGHT, parent);
    mScene->setSceneRect(QRectF(0, 0, GAME_WIDTH, GAME_HEIGHT));

    mDeck = new Deck(showDeck, nullptr);
    mDeck->setPos(CARD_SPACING+CARD_WIDTH/2, GAME_HEIGHT/2);
    mScene->addItem(mDeck);

    // Create our cards
    for (Suite suite: SuiteIterator() ) {
        for (CardValue value: CardValueIterator()) {

            Card *item = new Card(value, suite, nullptr);
            item->setPos(30+(double)(value)*30.0, 3*CARD_HEIGHT/2+2*TOP_MARGIN+(double)suite*22);

            bool result = QObject::connect(item, &Card::clicked, this, &Game::onCardClicked);
            mScene->addItem(item);
            mDeck->addCard(*item);
        }
    }

    mHand = new RandomStack(nullptr);
    mHand->setPos(CARD_SPACING+CARD_WIDTH/2, TOP_MARGIN+CARD_HEIGHT/2);
    mScene->addItem(mHand);

    mWastePile = new RandomStack(nullptr);
    mWastePile->setPos(2*CARD_SPACING+ 3*CARD_WIDTH/2, TOP_MARGIN+CARD_HEIGHT/2);
    mScene->addItem(mWastePile);

    // Create Sorted Stacks to drop cards into to win (Ace -> King, only one suite)
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

    // Create clickable links along the bottom
    qreal textWidth = 0;
    for(int i = 0; i < 3; i++) {
        ClickableGraphicsTextItem* item = new ClickableGraphicsTextItem(nullptr);
        switch (i) {
        case 0:
            item->setPlainText(QString("Shuffle"));
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onShuffleClicked);
            break;
        case 1:
            item->setPlainText(QString{"Deal"});
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onDealClicked);
            break;
        case 2:
            item->setPlainText(QString{"Exit"});
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onExitClicked);
            break;
        default:
            break;
        }
        item->adjustSize();
        item->setPos(2*GAME_WIDTH/10+textWidth, GAME_HEIGHT-CARD_HEIGHT);
        textWidth += item->textWidth() + CARD_SPACING;
        item->setDefaultTextColor(Qt::white);
        item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
        item->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
        mScene->addItem(item);
    }

    this->setScene(mScene);
}

void Game::showEvent(QShowEvent *event)
{

    this->setRenderHint(QPainter::Antialiasing);
    this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    this->setBackgroundBrush(QColor(22, 161, 39));      // A medium dark green
}

void Game::onCardClicked(Card& card)
{
    qDebug() << "Clicked" << card.getText();
}

void Game::onShuffleClicked()
{
    qDebug() << __func__;
    mDeck->shuffle();
}

void Game::onDealClicked()
{
    qDebug() << __func__;
}

void Game::onExitClicked()
{
    qDebug() << __func__;

    // https://doc.qt.io/qt-6/qmessagebox.html
    QMessageBox* msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("Exit Solitaire"));
    msgBox->setText(tr("Exit Program?"));
    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setDefaultButton(QMessageBox::Save);

    // Exec returns a QStandardButton value, note that pressing Esc returns "No"
    int ret = msgBox->exec();
    switch (ret) {
    case QMessageBox::Yes:
        QApplication::exit(0);
        break;
    case QMessageBox::No:
        qDebug() << "Exit Canceled";
        break;
    default:
        // should never be reached
        break;
    }
}
