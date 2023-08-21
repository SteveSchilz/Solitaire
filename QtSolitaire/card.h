#ifndef CARD_H
#define CARD_H

#include "enumiterator.h"

#include <QChar>
#include <QColor>
#include <QGraphicsItem>

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

enum class Suite {
    HEART,
    DIAMOND,
    SPADE,
    CLUB
};
typedef enumIterator<Suite, Suite::HEART, Suite::CLUB> SuiteIterator;

enum class Colors {
    RED,
    BLACK
};
typedef enumIterator<Colors, Colors::RED, Colors::BLACK> ColorsIterator;

class Card : public QGraphicsItem
{
public:
    Q_DISABLE_COPY(Card)

    Card() = delete;
    Card(CardValue v, Suite s, QGraphicsItem *parent = nullptr);
    ~Card();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

 private:
    QChar getSuiteChar();
    const char *getText();
    const char *getImagePath();

    char value;

    QColor mColor;
    QColor mTextColor;
    QString mPaintText;
    QGraphicsSvgItem *mImage;
    CardValue mValue;
    Suite mSuite;
};

#endif // CARD_H
