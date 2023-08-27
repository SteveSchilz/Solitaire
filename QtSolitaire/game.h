#ifndef GAME_H
#define GAME_H

#include "card.h"
#include "constants.h"

#include <QGraphicsView>

// Forward Declarations
class CardStack;
class DescendingStack;
class Deck;
class myScene;

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game() = delete;
    Game(QWidget* parent);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void onCardClicked(Card& card);
    void onShuffleClicked();
    void onDealClicked();
    void onExitClicked();

private:
    myScene *mScene;
    Deck *mDeck;
    CardStack *mHand;
    CardStack *mWastePile;

    CardStack *mHearts;
    CardStack *mDiamonds;
    CardStack *mClubs;
    CardStack *mSpades;

    DescendingStack* mPlayStacks[NUM_PLAY_STACKS] ;
};

#endif // GAME_H
