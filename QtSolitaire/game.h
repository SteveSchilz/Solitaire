#ifndef GAME_H
#define GAME_H

#include "card.h"

#include <QGraphicsView>

// Forward Declarations
class CardStack;
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

private:
    myScene *mScene;
    Deck *mDeck;
    CardStack *mHand;
    CardStack *mWastePile;

    CardStack *mHearts;
    CardStack *mDiamonds;
    CardStack *mClubs;
    CardStack *mSpades;

};

#endif // GAME_H
