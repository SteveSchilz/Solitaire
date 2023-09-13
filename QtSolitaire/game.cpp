#include "game.h"

#include "card.h"
#include "cardstack.h"
#include "clickableitem.h"
#include "constants.h"
#include "deck.h"
#include "myscene.h"
#include "undocommands.h"

#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <QMessageBox>
#include <QMenuBar>
#include <QUndoCommand>

const bool showDeck{true};  //< Debug flag to show initial state of deck.
/**
 * @brief Game Constructor
 *
 * @param parent The central ui widget
 */
Game::Game(QWidget* parent, QMenuBar *menubar)
    : QGraphicsView{parent}
    , mScene{nullptr}
    , mDeck{nullptr}
    , mHand{nullptr}
    , mWastePile{nullptr}
    , mHearts{nullptr}
    , mDiamonds{nullptr}
    , mSpades{nullptr}
    , mClubs{nullptr}
    , mUndoStack{}
    , mMenuBar{menubar}
{
    mScene = new  myScene(0, 0, GAME_WIDTH, GAME_HEIGHT, parent);
    mScene->setSceneRect(QRectF(0, 0, GAME_WIDTH, GAME_HEIGHT));
    this->setScene(mScene);

    createDeck(mScene, &mDeck);                         // Create the deck
    createCards(mScene, mDeck);                         // Create the cards and place them in the deck
    createHandAndWaste(mScene, &mHand, &mWastePile);
    createFoundation(mScene, &mHearts, &mDiamonds, &mSpades, &mClubs);
    createPlayfield(mScene, &mPlayStacks[0]);
    createActions(mScene);
    createMenus();

    QObject::connect(&mUndoStack, &QUndoStack::cleanChanged, this, &Game::onCleanChanged);
    QObject::connect(&mUndoStack, &QUndoStack::canUndoChanged, this, &Game::onCanUndoChanged);
    QObject::connect(&mUndoStack, &QUndoStack::canRedoChanged, this, &Game::onCanRedoChanged);
    onCleanChanged(true);
}

void Game::showEvent(QShowEvent *event)
{

    this->setRenderHint(QPainter::Antialiasing);
    this->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    this->setBackgroundBrush(QColor(22, 161, 39));      // A medium dark green
 }

 /**
  * @brief Create the Deck
  *
  * @param scene The graphics scene
  * @param [out] deck - address of pointer to Deck holding the cards to be initialized
  */
void Game::createDeck(myScene *scene, Deck **deck) {
    (*deck) = new Deck(showDeck, nullptr);
    (*deck)->setPos(CARD_SPACING+CARD_WIDTH/2, GAME_HEIGHT/2);
    scene->addItem(mDeck);
}

/**
 * @brief Create the Cards
 *
 * @param scene The graphics scene
 * @param deck  pointer to Deck holding the cards that are created
 */
void Game::createCards(myScene *scene, Deck *deck) {
    if (!scene || !deck) {
        return;
    }
    // Create our cards
    for (Suit suit: SuitIterator() ) {
        for (CardValue value: CardValueIterator()) {

            Card *item = new Card(value, suit, nullptr);
            item->setPos(30+(double)(value)*30.0, 3*CARD_HEIGHT/2+2*TOP_MARGIN+(double)suit*22);

            QObject::connect(item, &Card::clicked, this, &Game::onCardClicked);
            QObject::connect(item, &Card::doubleClicked, this, &Game::onCardDoubleClicked);
            scene->addItem(item);
            deck->addCard(item);
        }
    }
}

/**
 * @brief Create the Hand and Waste Piles
 *
 * @param scene The graphics scene
 * @param [out] deck - address of pointer to the Hand Stack that holds leftover cards after the deal
 * @param [out] deck - address of pointer to the Waste pile Stack
 */
void Game::createHandAndWaste(myScene* scene, RandomStack **hand, RandomStack **wastePile)
{
    if (!scene || !hand || !wastePile) {
        return;
    }

    (*hand) = new RandomStack(nullptr);
    (*hand)->setPos(CARD_SPACING+CARD_WIDTH/2, TOP_MARGIN+CARD_HEIGHT/2);
    QObject::connect( (*hand), &CardStack::clicked, this, &Game::onEmptyHandClicked);

    scene->addItem( (*hand));

    (*wastePile) = new RandomStack(nullptr);
    (*wastePile)->setPos(2*CARD_SPACING+ 3*CARD_WIDTH/2, TOP_MARGIN+CARD_HEIGHT/2);
    scene->addItem((*wastePile));
}
/**
 *  @brief Create Sorted Stacks to drop cards into to win (Ace -> King, only one suit)
 *
 *  @param [out] hearts pointer to pointer to be initialized for Foundation Stack Hearts
 *  @param [out] diamonds pointer to pointer to be initialized for Foundation Stack Diamonds
 *  @param [out] spades pointer to pointer to be initialized for Foundation Stack Spades
 *  @param [out] clups pointer to pointer to be initialized for Foundation Stack Clubs
 */
void Game::createFoundation(myScene *scene, SortedStack **hearts, SortedStack **diamonds, SortedStack **spades, SortedStack **clubs)
{
    if (!scene || !hearts || !diamonds || !spades || !clubs) {
        return;
    }

    for (Suit suit: SuitIterator()) {
        SortedStack *stack = new SortedStack(suit, nullptr);
        stack->setPos(4*CARD_SPACING+3*CARD_WIDTH + CARD_WIDTH/2 + (double)suit*(CARD_WIDTH+CARD_SPACING), TOP_MARGIN+CARD_HEIGHT/2);
        stack->setTransform(QTransform::fromScale(1.0, 1.0), true);
        scene->addItem(stack);
        switch(suit) {
        case Suit::HEART: (*hearts) = stack; break;
        case Suit::DIAMOND: (*diamonds) = stack; break;
        case Suit::SPADE:  (*spades) = stack; break;
        case Suit::CLUB:   (*clubs) = stack; break;
        }
    }
}

/**
 * @brief Create the playing field
 *
 * @param[out] ptr-to-ptr to an array of DescendingStacks used to create the playfield
 */
void Game::createPlayfield(myScene *scene, pDStackArray stacks){

    if (!scene || !stacks) {
        return;
    }

    for (int i = 0; i < NUM_PLAY_STACKS; i++) {
        stacks[i] = new DescendingStack(nullptr);
        stacks[i]->setPos(CARD_SPACING + CARD_WIDTH/2 + (double)i*(CARD_WIDTH+CARD_SPACING), +TOP_MARGIN+ 3*CARD_HEIGHT/2 + CARD_SPACING);
        scene->addItem(mPlayStacks[i]);
    }
}
/*
 * @brief Create menu actions and clickable links along the bottom
 */
void Game::createActions(myScene *scene)
{
    qreal textWidth = 0;
    for(int i = 0; i < 6; i++) {
        ClickableGraphicsTextItem* item = new ClickableGraphicsTextItem(nullptr);
        QAction *action = new QAction();
        switch (i) {
        case 0:
        {
            QString undoStr{tr("Undo")};
            item->setPlainText(undoStr);
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onUndoClicked);

            undoAction = action;
            undoAction->setText(undoStr);
            undoAction->setShortcut(tr("Ctrl+Z"));
            QObject::connect(undoAction, &QAction::triggered, this, &Game::onUndoClicked);
        }
        break;
        case 1:
        {
            QString redoStr(tr("Redo"));
            item->setPlainText(redoStr);
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onRedoClicked);

            redoAction = action;
            redoAction->setText(redoStr);
            redoAction->setShortcut(tr("Shift+Ctrl+Z"));
            QObject::connect(redoAction, &QAction::triggered, this, &Game::onRedoAction);
        }
        break;
        case 2:
        {
            QString shuffleStr(tr("Shuffle"));
            item->setPlainText(shuffleStr);
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onShuffleClicked);

            shuffleAction = action;
            shuffleAction->setText(shuffleStr);
            shuffleAction->setShortcut(tr("Ctrl+S"));
            QObject::connect(shuffleAction, &QAction::triggered, this, &Game::onShuffleAction);
        }
        break;
        case 3:
        {
            QString dealString(tr("Deal"));
            item->setPlainText(dealString);
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onDealClicked);

            dealAction = action;
            dealAction->setText(dealString);
            dealAction->setShortcut(tr("Ctrl+D"));
            QObject::connect(dealAction, &QAction::triggered, this, &Game::onDealAction);
        }
        break;
        case 4:
        {
            QString newGameString(tr("New Game"));
            item->setPlainText(newGameString);
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onNewGameClicked);

            newGameAction = action;
            newGameAction->setText(newGameString);
            newGameAction->setShortcut(tr("Ctrl+N"));
            QObject::connect(newGameAction, &QAction::triggered, this, &Game::onNewGameAction);
        }
        break;

        case 5:
        {
            QString exitString(tr("Exit"));
            item->setPlainText(exitString);
            QObject::connect(item, &ClickableGraphicsTextItem::clicked, this, &Game::onExitClicked);

            exitAction = action;
            exitAction->setText(exitString);
            exitAction->setShortcut(tr("Ctrl+Q"));
            QObject::connect(exitAction, &QAction::triggered, this, &Game::onExitAction);
        }
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
        scene->addItem(item);
    }
}

void Game::createMenus() {

    QMenuBar* fileMenu = reinterpret_cast<QMenuBar*>(mMenuBar->findChild<QObject*>("menuSolitaire"));
    if (fileMenu) {
        fileMenu->addAction(undoAction);
        fileMenu->addAction(redoAction);
        fileMenu->addAction(shuffleAction);
        fileMenu->addAction(dealAction);
        fileMenu->addAction(newGameAction);
        fileMenu->addAction(exitAction);
    }
}

void Game::onUndoAction(bool checked) {
    Q_UNUSED(checked);
    onUndoClicked();
}

void Game::onUndoClicked()
{
    QUndoCommand *undo{nullptr};

    if (mUndoStack.canUndo()) {
        mUndoStack.undo();
    }
}

void Game::onRedoAction(bool checked) {
    Q_UNUSED(checked);
    onRedoClicked();
}

void Game::onRedoClicked()
{
    if (mUndoStack.canRedo()) {
        mUndoStack.redo();
    }
}

void Game::onCleanChanged(bool clean) {

    undoAction->setEnabled(mUndoStack.canUndo());
    redoAction->setEnabled(mUndoStack.canRedo());
}

void Game::onCanUndoChanged(bool canUndo)
{
        undoAction->setEnabled(canUndo);
}
void Game::onCanRedoChanged(bool canRedo)
{
        redoAction->setEnabled(canRedo);
}

/**
 * @brief Game::onEmptyHandClicked -
 * @param stack
 */
void Game::onEmptyHandClicked(CardStack& stack)
{
    if( !mWastePile || mWastePile->isEmpty()) {
        return;
    }
    if (&stack == mHand) {
        mUndoStack.push(new ResetHandCommand(mHand, mWastePile));
    }
}

void Game::onCardClicked(Card& card)
{
    // Cards on the hand can move to the waste pile
    if (card.parentItem() == mHand) {
        HandToWasteCommand *command = new HandToWasteCommand(mHand, mWastePile);
        mUndoStack.push(command);
    }

}

void Game::onCardDoubleClicked(Card& card)
{
    // Move card from Waste Pile to main playfield
    if (card.parentItem() == mWastePile) {
        for (int i = 0; i < NUM_PLAY_STACKS; ++i) {
            if (mPlayStacks[i]->canAdd(card)) {
                MoveToPlayfieldCommand *command = new MoveToPlayfieldCommand(mWastePile, mPlayStacks[i]);
                mUndoStack.push(command);
                break;
            }
        }
    }

    // Move card from Waste Pile to Sorted Stacks Per-Suit)
    SortedStack* sStack{nullptr};
    for (Suit suit: SuitIterator()) {
        switch(suit) {
        case Suit::HEART: sStack = mHearts; break;
        case Suit::DIAMOND: sStack = mDiamonds; break;
        case Suit::CLUB:   sStack = mClubs; break;
        case Suit::SPADE:  sStack = mSpades; break;
        }

        if (sStack->canAdd(card)) {
            if (card.parentItem() == mWastePile) {
                WasteToFoundationCommand *command = new WasteToFoundationCommand(mWastePile, sStack);
                mUndoStack.push(command);
            }
        }

        // Check if card can move from main playfield to the Foundation Piles (Sorted Stacks)
        for (int i = 0; i < NUM_PLAY_STACKS; ++i) {
            // TODO: Ensure that only top card moves (perhaps add "canTake" method?)
            if (card.parentItem() == mPlayStacks[i] && sStack->canAdd(card)) {
                PlayfieldToFoundationCommand *command = new PlayfieldToFoundationCommand(mPlayStacks[i], sStack);
                mUndoStack.push(command);
                break;
            }
            // Handle cards moving from one of the main playfield stacks to a different main playfield stack
            for (int j = 0; j < NUM_PLAY_STACKS; ++j) {
                if (i == j) {
                    continue;
                }
                if (card.parentItem() == mPlayStacks[i] && mPlayStacks[j]->canAdd(card)) {
                    PlayfieldToPlayfieldCommand *command = new PlayfieldToPlayfieldCommand(mPlayStacks[i], mPlayStacks[j]);
                    mUndoStack.push(command);
                    break;
                }
            }
        }
    }
}

static FanDirection direction = FanDirection::FOUR_ROWS;

void Game::onShuffleAction(bool checked) {
    Q_UNUSED(checked);
    onShuffleClicked();
}

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

void Game::onDealAction(bool checked) {
    Q_UNUSED(checked);
    onDealClicked();
}

void Game::onDealClicked()
{
    qDebug() << __func__;
    // This check is sufficent, because the deck is either full or empty at all times, except
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

    // Flip the top card over and put it on the waste pile
    card = mHand->takeTop();
    card->setFaceUp(true);
    mWastePile->addCard(card);

}

void Game::onNewGameAction(bool checked) {
    Q_UNUSED(checked);
    onNewGameClicked();
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
    SortedStack *sStack{nullptr};

    for (Suit suit: SuitIterator()) {
        switch(suit) {
        case Suit::HEART: sStack = mHearts; break;
        case Suit::DIAMOND: sStack = mDiamonds; break;
        case Suit::CLUB:   sStack = mClubs; break;
        case Suit::SPADE:  sStack = mSpades; break;
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
    mUndoStack.setClean();
}

void Game::onExitAction(bool checked) {
    Q_UNUSED(checked);
    onExitClicked();
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
