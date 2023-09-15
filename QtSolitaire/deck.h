#ifndef DECK_H
#define DECK_H
#include "card.h"
#include "cardstack.h"
#include <QGraphicsObject>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QUndoStack);
/**
 * @brief The Deck class is an adaption of RandomStack used to temporarily hold the cards when shuffling the deck and dealing.
 *
 */
class Deck : public RandomStack
{
    Q_OBJECT
public:

    Deck() = delete;
    Deck(bool showDeck, QGraphicsItem *parent = nullptr, QUndoStack *undoStack = nullptr);
    ~Deck();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    bool isEmpty() const override { return mCards.isEmpty(); }
    void addCard(Card* card, bool flipTop) override;
    void shuffle();
    Card* deal();

private:
    bool mShowDeck;
};


#endif // DECK_H

