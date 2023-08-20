#ifndef CARD_H
#define CARD_H

#include <QColor>
#include <QGraphicsItem>

QT_FORWARD_DECLARE_CLASS(QGraphicsSvgItem)

enum CardValue {
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

enum Suite {
    HEART,
    DIAMOND,
    SPADE,
    CLUB
};

enum Colors {
    RED,
    BLACK
};

class Card : public QGraphicsItem
{
public:
    Q_DISABLE_COPY(Card)

    Card() = delete;
    Card(CardValue v, Suite s, QGraphicsItem *parent = nullptr);
    ~Card();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void setImage(QString path);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    const char *getText();
 private:
    char value;

    QColor mColor;
    QColor mTextColor;
    QGraphicsSvgItem *mImage;
    CardValue mValue;
    Suite mSuite;
};

#endif // CARD_H
