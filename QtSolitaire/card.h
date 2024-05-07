#ifndef CARD_H
#define CARD_H

#include "enumiterator.h"

#include <QChar>
#include <QColor>
#include <QGraphicsObject>

QT_FORWARD_DECLARE_CLASS(QGraphicsSvgItem)

enum class CardValue {
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};
typedef enumIterator<CardValue, CardValue::ACE, CardValue::KING> CardValueIterator;

enum class Suit {
    HEART,
    DIAMOND,
    SPADE,
    CLUB
};
typedef enumIterator<Suit, Suit::HEART, Suit::CLUB> SuitIterator;

enum class Colors {
    RED,
    BLACK
};
typedef enumIterator<Colors, Colors::RED, Colors::BLACK> ColorsIterator;

class Card : public QGraphicsObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Card)
public:

    Card() = delete;
    Card(CardValue v, Suit s, QGraphicsItem *parent = nullptr);
    ~Card();

    bool isFaceUp() const { return mFaceUp; }
    void setFaceUp(bool faceUp);

    Suit getSuit() const { return mSuit; }
    QColor getColor() const { return mColor; }
    CardValue getValue() const { return mValue; }
    const QString& getText() const { return mPaintText; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

signals:
    void clicked(Card& card);
    void doubleClicked(Card& card);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;


 private:
    QChar getSuitChar();
    const char *getValueText();
    const char *getImagePath();

    bool mFaceUp;             ///< True if card face is showing, otherwise back of card is visible
    bool mMouseDown;
    bool mHover;
    QColor mColor;
    QString mPaintText;
    QGraphicsSvgItem *mImage;           // Note: these are deleted via being children of the card ;)
    QGraphicsSvgItem *mBackImage;       //       Refer to https://doc.qt.io/qt-6/objecttrees.html if
                                              //       you are unfamiliar with qt memory management

    CardValue mValue;
    Suit mSuit;
};

QDataStream & operator << (QDataStream & s, const Card *cardptr);
QDataStream & operator >> (QDataStream & s, Card *& cardptr);

#endif // CARD_H
