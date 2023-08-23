#include "game.h"

#include "card.h"
#include "cardstack.h"
#include "constants.h"
#include "deck.h"
#include "myScene.h"

#include <QGraphicsView>
const bool showDeck{true};

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

    this->setScene(mScene);
}

void Game::showEvent(QShowEvent *event)
{

    this->setRenderHint(QPainter::Antialiasing);
    this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    this->setBackgroundBrush(QColor(22, 161, 39));      // A medium dark green
}
