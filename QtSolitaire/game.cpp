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
    , mUndoStack{}

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

            QObject::connect(item, &Card::clicked, this, &Game::onCardClicked);
            QObject::connect(item, &Card::doubleClicked, this, &Game::onCardDoubleClicked);
            mScene->addItem(item);
            mDeck->addCard(item);
        }
    }

    mHand = new RandomStack(nullptr);
    mHand->setPos(CARD_SPACING+CARD_WIDTH/2, TOP_MARGIN+CARD_HEIGHT/2);
    QObject::connect(mHand, &CardStack::clicked, this, &Game::onHandClicked);

    mScene->addItem(mHand);

    mWastePile = new RandomStack(nullptr);
    mWastePile->setPos(2*CARD_SPACING+ 3*CARD_WIDTH/2, TOP_MARGIN+CARD_HEIGHT/2);
    mScene->addItem(mWastePile);

    // Create Sorted Stacks to drop cards into to win (Ace -> King, only one suite)
    for (Suite suite: SuiteIterator()) {
        SortedStack *stack = new SortedStack(suite, nullptr);
        stack->setPos(4*CARD_SPACING+3*CARD_WIDTH + CARD_WIDTH/2 + (double)suite*(CARD_WIDTH+CARD_SPACING), TOP_MARGIN+CARD_HEIGHT/2);
        stack->setTransform(QTransform::fromScale(1.0, 1.0), true);
        mScene->addItem(stack);
        switch(suite) {
        case Suite::HEART: mHearts = stack; break;
        case Suite::DIAMOND: mDiamonds = stack; break;
        case Suite::CLUB:   mClubs = stack; break;
        case Suite::SPADE:  mSpades = stack; break;
        }
    }

    // Create the playing field
    for (int i = 0; i < NUM_PLAY_STACKS; i++) {
        mPlayStacks[i] = new DescendingStack(nullptr);
        mPlayStacks[i]->setPos(CARD_SPACING + CARD_WIDTH/2 + (double)i*(CARD_WIDTH+CARD_SPACING), +TOP_MARGIN+ 3*CARD_HEIGHT/2 + CARD_SPACING);
        mScene->addItem(mPlayStacks[i]);
    }
    // Create clickable links along the bottom
    qreal textWidth = 0;
    for(int i = 0; i < 6; i++) {
        ClickableGraphicsTextItem* item = new ClickableGraphicsTextItem(nullptr);
        switch (i) {
        case 0:
            item->setPlainText(QString("Undo"));
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onUndoClicked);
            break;
        case 1:
            item->setPlainText(QString("Redo"));
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onRedoClicked);
            break;
        case 2:
            item->setPlainText(QString("Shuffle"));
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onShuffleClicked);
            break;
        case 3:
            item->setPlainText(QString{"Deal"});
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onDealClicked);
            break;
        case 4:
            item->setPlainText(QString{"New Game"});
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onNewGameClicked);
            break;

        case 5:
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

void Game::onUndoClicked()
{
    qDebug() << "Undo!";
}

void Game::onRedoClicked()
{
    qDebug() << "Redo!";
}

void Game::onHandClicked(CardStack& stack)
{
    Card *card{nullptr};

    if( !mWastePile || mWastePile->isEmpty()) {
        return;
    }
    while (!mWastePile->isEmpty()) {
        card = mWastePile->takeTop();
        card->setFaceUp(false);
        mHand->addCard(card);
    }

}

void Game::onCardClicked(Card& card)
{
    Card *topCard{nullptr};
    qDebug() << "Clicked" << card.getText() << "Parent" << card.parentItem();
    if (card.parentItem() == mHand) {
        topCard = mHand->takeTop();
        topCard->setFaceUp(true);
        mWastePile->addCard(topCard);
        if (topCard->getValue() != card.getValue()) {
            qDebug() << "SOMETHING IS WRONG IN ONCARD CLICKED!";
        }
    }
}

void Game::onCardDoubleClicked(Card& card)
{
    // Move card from Waste Pile to main playfield
    if (card.parentItem() == mWastePile) {
        for (int i = 0; i < NUM_PLAY_STACKS; ++i) {
            if (mPlayStacks[i]->canAdd(card)) {
                mWastePile->takeCard(&card);
                mPlayStacks[i]->addCard(&card);
                break;
            }
        }
    }

    // Move card from Waste Pile to Sorted Stacks Per-Suit)
    CardStack* sStack{nullptr};
    for (Suite suite: SuiteIterator()) {
        switch(suite) {
        case Suite::HEART: sStack = mHearts; break;
        case Suite::DIAMOND: sStack = mDiamonds; break;
        case Suite::CLUB:   sStack = mClubs; break;
        case Suite::SPADE:  sStack = mSpades; break;
        }

        if (sStack->canAdd(card)) {
            if (card.parentItem() == mWastePile) {
                mWastePile->takeCard(&card);
                sStack->addCard(&card);
            }
        }

        // Check if card can move from main playfield to the Foundation Piles (Sorted Stacks)
        for (int i = 0; i < NUM_PLAY_STACKS; ++i) {
            // TODO: Ensure that only top card moves (perhaps add "canTake" method?)
            if (card.parentItem() == mPlayStacks[i] && sStack->canAdd(card)) {
                mPlayStacks[i]->takeCard(&card);
                sStack->addCard(&card);
                break;
            }
            // Handle cards moving from one of the main playfield stacks to a different main playfield stack
            for (int j = 0; j < NUM_PLAY_STACKS; ++j) {
                if (i == j) {
                    continue;
                }
                if (card.parentItem() == mPlayStacks[i] && mPlayStacks[j]->canAdd(card)) {
                    mPlayStacks[i]->takeCard(&card);
                    mPlayStacks[j]->addCard(&card);
                    break;
                }
            }
        }
    }
}

static FanDirection direction = FanDirection::FOUR_ROWS;

void Game::onShuffleClicked()
{
    qDebug() << __func__;
    mDeck->shuffle();
    mDeck->fanCards(direction);
    if (direction == FanDirection::FOUR_ROWS) {
        direction = FanDirection::HORIZONTAL;
    } else if (direction == FanDirection::HORIZONTAL) {
        direction = FanDirection::VERTICAL;
    } else {
        direction = FanDirection::FOUR_ROWS;
    }
    update();
}

void Game::onDealClicked()
{
    qDebug() << __func__;
    // This chekc is sufficent, because the deck is either full or empty at all times, except
    // during the excution of this method.
    if (!mDeck || mDeck->isEmpty()) {
        return;
    }

    int baseStack = 0;                              // Start at stack 0, go one heigher each iteration:
    Card *card{nullptr};

    // Deal the cards onto the tableau
    for (int i = 0; i < NUM_PLAY_STACKS; ++i) {
        for (int j = baseStack; j < NUM_PLAY_STACKS; ++j) {
            card = mDeck->deal();
            mPlayStacks[j]->addCard(card);
            if (i < j) {
                card->setFaceUp(false);
           }
        }
        baseStack++;
    }

    // Move the remaining cards to the hand
    while (! mDeck->isEmpty()) {
        card = mDeck->deal();
        card->setFaceUp(false);
        mHand->addCard(card);        
    }

    // Flip the top card over
    card = mHand->takeTop();
    qDebug() << "Top Card is " << card->getText() << "parent is: " << card->parentItem();
    card->setFaceUp(true);
    mWastePile->addCard(card);

}

void Game::onNewGameClicked()
{
    qDebug() << __func__;

    Card *card{nullptr};

    // Recover cards on the hand
    while (!mHand->isEmpty()) {
        card = mHand->takeTop();
        mDeck->addCard(card);
        card->setFaceUp(true);
    }

    // Recover cards on the Waste Pile
    while (!mWastePile->isEmpty()) {
        card = mWastePile->takeTop();
        mDeck->addCard(card);
        card->setFaceUp(true);
    }

    // Recover the Foundation (Sorted Stacks)
    CardStack *sStack{nullptr};

    for (Suite suite: SuiteIterator()) {
        switch(suite) {
        case Suite::HEART: sStack = mHearts; break;
        case Suite::DIAMOND: sStack = mDiamonds; break;
        case Suite::CLUB:   sStack = mClubs; break;
        case Suite::SPADE:  sStack = mSpades; break;
        }
        while (!sStack->isEmpty()) {
           card = sStack->takeTop();
           mDeck->addCard(card);
           card->setFaceUp(true);
        }
    }

    // Recover cards on the tableau
    for (int i = 0; i < NUM_PLAY_STACKS; ++i) {
        while (!mPlayStacks[i]->isEmpty()) {
           card = mPlayStacks[i]->takeTop();
           mDeck->addCard(card);
           card->setFaceUp(true);
        }
    }

    mHand->newGame();
    mWastePile->newGame();
    mHearts->newGame();
    mDiamonds->newGame();
    mSpades->newGame();
    mClubs->newGame();
    for (int i =0; i < NUM_PLAY_STACKS; ++i) {
        mPlayStacks[i]->newGame();
    }
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
