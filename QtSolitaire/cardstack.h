#ifndef CARDSTACK_H
#define CARDSTACK_H

#include "card.h"

#include <QObject>
#include <QGraphicsItem>
#include <QStack>

QT_FORWARD_DECLARE_CLASS(QGraphicsSvgItem)


class CardStack : public QGraphicsItem
{
public:
    Q_DISABLE_COPY(CardStack)

    CardStack() = delete;
    CardStack(QGraphicsItem *parent = nullptr);
    ~CardStack();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    QColor mColor = Qt::lightGray;
    bool mDragOver = false;
};

/*!
 * @class SortedStack
 * @brief The SortedStack class is used for the final destination for cards
 * The cards must satisfy the following characteristics:
 * - All cards must have the same Suite
 * - The stack is populated starting with an Ace, and increasing from there to a King
 *
 */
class SortedStack: public CardStack {
public:
    SortedStack() = delete;
    SortedStack(Suite s, QGraphicsItem *parent = nullptr);
    ~SortedStack();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    const char *getImagePath(Suite s);

    bool mDropAccepted;
    QStack<Card*> mCards;
    Suite mSuite;
    QGraphicsSvgItem *mImage;
};


/*!
 * @class RandomStack
 * @brief The RandomStack class is used for the shuffled "hand" stack and the associated "wastepile"
 * There are no special rules for these cards
 */
class RandomStack: public CardStack {
public:
    RandomStack() = delete;
    RandomStack(QGraphicsItem *parent = nullptr);
    ~RandomStack();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:

private:
    const char *getImagePath();

    bool mDropAccepted;
    QList<Card*> mCards;
    QGraphicsSvgItem *mImage;

};

#endif // CARDSTACK_H
