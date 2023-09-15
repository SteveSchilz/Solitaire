#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "cardstack.h"
#include "constants.h"

#include <QGraphicsView>
#include <QSharedPointer>
#include <QUndoStack>

// Forward Declarations
class CardStack;
class DescendingStack;
class RandomStack;
class SortedStack;
class Deck;
class myScene;

QT_FORWARD_DECLARE_CLASS(QMenuBar);

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game() = delete;
    Game(QWidget* parent, QMenuBar *menubar);

    void createDeck(myScene *scene, Deck** deck);
    void createCards(myScene *scene, Deck *deck);
    void createHandAndWaste(myScene *scene, RandomStack **hand, RandomStack **wastePile);
    void createFoundation(myScene *scene, SortedStack **hearts, SortedStack **diamonds, SortedStack **Spades, SortedStack **clubs);
    void createPlayfield(myScene *scene, pDStackArray stacks);
    void createActions(myScene *scene);
    void createMenus();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onEmptyHandClicked(CardStack& stack);
    void onCardClicked(Card& card);
    void onCardDoubleClicked(Card& card);
    void onShuffleClicked();
    void onDealClicked();
    void onNewGameClicked();
    void onExitClicked();

    void onUndoAction(bool checked=false);
    void onRedoAction(bool checked=false);
    void onCleanChanged(bool clean);
    void onCanUndoChanged(bool canUndo);
    void onCanRedoChanged(bool canRedo);

    void onShuffleAction(bool checked=false);
    void onDealAction(bool checked=false);
    void onNewGameAction(bool checked=false);
    void onExitAction(bool checked=false);

private:
    myScene *mScene;
    Deck *mDeck;
    RandomStack *mHand;
    RandomStack *mWastePile;

    SortedStack *mHearts;
    SortedStack *mDiamonds;
    SortedStack *mClubs;
    SortedStack *mSpades;

    DescendingStack* mPlayStacks[NUM_PLAY_STACKS] ;

    QAction *undoAction;
    QAction *redoAction;
    QAction *shuffleAction;
    QAction *dealAction;
    QAction *newGameAction;
    QAction *exitAction;
    QMenuBar *mMenuBar;

    QSharedPointer<QUndoStack>mUndoStack;
};

#endif // GAME_H
