#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>

// Forward Declarations
class CardStack;
class myScene;

class Game : public QGraphicsView
{
public:
    Game() = delete;
    Game(QWidget* parent);

protected:
    void showEvent(QShowEvent *event) override;

private:
    myScene *mScene;
    CardStack *mHearts;
    CardStack *mDiamonds;
    CardStack *mClubs;
    CardStack *mSpades;

};

#endif // GAME_H
