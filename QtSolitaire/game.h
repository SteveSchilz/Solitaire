#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "cardstack.h"
#include "constants.h"

#include <QGraphicsView>
#include <QUndoStack>

// Forward Declarations
class CardStack;
class DescendingStack;
class RandomStack;
class Deck;
class myScene;

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game() = delete;
    Game(QWidget* parent);

    void createDeck(myScene *scene, Deck** deck);
    void createCards(myScene *scene, Deck *deck);
    void createHandAndWaste(myScene *scene, RandomStack **hand, RandomStack **wastePile);
    void createFoundation(myScene *scene, CardStack **hearts, CardStack **diamonds, CardStack **Spades, CardStack **clubs);
    void createPlayfield(myScene *scene, pDStackArray stacks);
    void createActions(myScene *scene);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onHandClicked(CardStack& stack);
    void onCardClicked(Card& card);
    void onCardDoubleClicked(Card& card);
    void onShuffleClicked();
    void onDealClicked();
    void onNewGameClicked();
    void onExitClicked();

private:
    myScene *mScene;
    Deck *mDeck;
    RandomStack *mHand;
    RandomStack *mWastePile;

    CardStack *mHearts;
    CardStack *mDiamonds;
    CardStack *mClubs;
    CardStack *mSpades;

    DescendingStack* mPlayStacks[NUM_PLAY_STACKS] ;

    QUndoStack mUndoStack;
};

#endif // GAME_H
