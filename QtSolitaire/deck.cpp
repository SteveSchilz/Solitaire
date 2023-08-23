#include "deck.h"
#include "constants.h"

#include <QPainter>

Deck::Deck(bool showDeck, QGraphicsItem *parent)
    :RandomStack(parent)
    ,mShowDeck(showDeck)
{

}

Deck::~Deck()
{

}

QRectF Deck::boundingRect() const
{
    double cardCount = (double)mCards.size()+1.0;
    QSizeF size{CARD_WIDTH+CARD_SPACING, (CARD_HEIGHT+CARD_SPACING)*cardCount};
    return QRectF(this->scenePos(), size);
}

void Deck::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);
    painter->restore();
}

void Deck::addCard(Card& card)
{
    RandomStack::addCard(card);
    double x = (double)((mCards.size()-1) % 13) * 30.0;
    int y = (double)((mCards.size()-1) / 13) * 30.0;
    card.setPos(x, y);
}

void Deck::shuffle()
{

}

Card* Deck::deal()
{
    Card * result = {nullptr};
    if (mCards.size() > 0) {
        result = mCards.takeFirst();
    }
    return result;
}

