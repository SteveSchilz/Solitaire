#ifndef CARDSTACK_H
#define CARDSTACK_H

#include "card.h"

#include <QAbstractAnimation>
#include <QObject>
#include <QGraphicsItem>
#include <QStack>

QT_FORWARD_DECLARE_CLASS(QGraphicsSvgItem)

/**
 * @brief The FanDirection enum selects a direction when laying out the cards
 */
enum class FanDirection {
    FOUR_ROWS,              ///< Show all four suits in a rectangualr array (good fo sorted deck of cards)
    HORIZONTAL,             ///< Horizontal row of cards
    VERTICAL                ///< Vertical row of cards
};

class CardStack : public QGraphicsObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CardStack)
public:

    CardStack() = delete;
    CardStack(QGraphicsItem *parent = nullptr);
    ~CardStack();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual bool canAdd(Card& card) const = 0;
    virtual void fanCards(FanDirection dir) = 0;  ///< Fan out the cards across a region of the table

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
 * Cards are "fanned" vertically from the stack position downwards.
 */
class SortedStack: public CardStack {

    friend class DescendingStack;

    Q_OBJECT
    Q_DISABLE_COPY(SortedStack)

public:

    SortedStack() = delete;
    SortedStack(Suite s, QGraphicsItem *parent = nullptr);
    ~SortedStack();

    virtual bool canAdd(Card& card) const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    virtual void fanCards(FanDirection dir) override;

protected slots:
    void fanAnimationFinished();

private:
    const char *getImagePath(Suite s);

    QStack<Card*> mCards;
    Suite mSuite;
    QGraphicsSvgItem *mImage;
};

/*!
 * @class DescendingStack
 * @brief DescendingStack class is used for the main playfield
 *
 * Cards are initially randomly arranged, with the top card face up, and others face down
 * New cards may be dropped on the stack must satisfy the following conditions
 *  a) Card must have the opposite color value
 *  b) Card must be one value Lower than top face up card
 */
class DescendingStack: public CardStack {
    Q_OBJECT
    Q_DISABLE_COPY(DescendingStack)

public:

    DescendingStack() = delete;
    DescendingStack(DescendingStack&) = delete;

    DescendingStack(QGraphicsItem *parent = nullptr);
    ~DescendingStack();

    virtual bool canAdd(Card& card) const override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void addCard(Card* card);
    double getYOffset() const;

    virtual void fanCards(FanDirection dir) override;

protected slots:
    void fanAnimationFinished();
    void onUpdateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

protected:

    QStack<Card*> mCards;

};


/*!
 * @class RandomStack
 * @brief The RandomStack class is used for the shuffled "hand" stack and the associated "wastepile"
 * There are no special rules for these cards
 */
class RandomStack: public CardStack {

    friend class DescendingStack;

    Q_OBJECT
    Q_DISABLE_COPY(RandomStack)

public:

    RandomStack() = delete;
    RandomStack(RandomStack&) = delete;

    RandomStack(QGraphicsItem *parent = nullptr);
    ~RandomStack();

    virtual bool canAdd(Card& card) const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void addCard(Card *card);
    Card *takeCard();

    virtual void fanCards(FanDirection dir) override;

protected slots:
    void fanAnimationFinished();
    void onUpdateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

protected:

    const char *getImagePath();

    QList<Card*> mCards;
    QGraphicsSvgItem *mImage;

};

#endif // CARDSTACK_H
