#ifndef CARDSTACK_H
#define CARDSTACK_H

#include "card.h"

#include <QAbstractAnimation>
#include <QObject>
#include <QGraphicsItem>
#include <QSharedPointer>
#include <QStack>

QT_FORWARD_DECLARE_CLASS(QGraphicsSvgItem)
QT_FORWARD_DECLARE_CLASS(QUndoStack)

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
    CardStack(QGraphicsItem *parent = nullptr, QUndoStack *undoStack  = nullptr);
    ~CardStack();

    virtual void newGame() = 0;

    virtual bool canAdd(Card& card) const = 0;
    virtual void addCard(Card *card, bool flipTop);

    bool canTake(Card& card) const;
    virtual Card* takeCard(Card *card);
    virtual Card* takeTop();

    /* topFlipped property helps with undo - when a card is removed from a stack the topFlipped variable may be set.
     *  when this property is set and an undo command is executed, then we know to flip it back.
     *
     *  This property is valid to read only immediately after a card is removed from the stack.
     */
    virtual bool isTopFlipped() const { return mTopFlipped; }
    virtual void setTopFlipped(bool flipped) { mTopFlipped = flipped;}

    bool isEmpty() { return mCards.isEmpty(); }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual void fanCards(FanDirection dir) = 0;  ///< Fan out the cards across a region of the table

signals:
    void clicked(CardStack& stack);

protected:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    QColor mColor;
    bool mDragOver;
    bool mMouseDown;
    bool mTopFlipped;
    QSharedPointer <QUndoStack>mUndoStack;
    QList<Card*> mCards;
};

/*!
 * @class SortedStack
 * @brief The SortedStack class is used for the final destination for cards
 * The cards must satisfy the following characteristics:
 * - All cards must have the same Suit
 * - The stack is populated starting with an Ace, and increasing from there to a King
 *
 * Cards are "fanned" vertically from the stack position downwards.
 */
class SortedStack: public CardStack {
    Q_OBJECT
    Q_DISABLE_COPY(SortedStack)

public:

    SortedStack() = delete;
    SortedStack(Suit s, QGraphicsItem *parent = nullptr, QUndoStack *undoStack  = nullptr);
    ~SortedStack();

    virtual void newGame() override;

    virtual bool canAdd(Card& card) const override;
    virtual void addCard(Card *card, bool flipTop) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    virtual void fanCards(FanDirection dir) override;

protected slots:
    void fanAnimationFinished();

private:
    const char *getImagePath(Suit s);

    Suit mSuit;
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

    DescendingStack(QGraphicsItem *parent = nullptr, QUndoStack *undoStack  = nullptr);
    ~DescendingStack();

    virtual void newGame() override;

    virtual bool canAdd(Card& card) const override;
    virtual void addCard(Card *card, bool flipTop) override;
    virtual Card* takeTop() override;
    QList<Card*> takeCards(Card& card);

    double getYOffset() const;


    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    virtual void fanCards(FanDirection dir) override;

protected slots:
    void fanAnimationFinished();
    void onUpdateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

protected:

};

typedef DescendingStack* pDStack;   ///< Pointer to DescendingStack
typedef DescendingStack* pDStackArray[];     ///< Array of pointers to DescendingStack

/*!
 * @class RandomStack
 * @brief The RandomStack class is used for the shuffled "hand" stack and the associated "wastepile"
 * There are no special rules for these cards
 */
class RandomStack: public CardStack {

    Q_OBJECT
    Q_DISABLE_COPY(RandomStack)

public:

    RandomStack() = delete;
    RandomStack(RandomStack&) = delete;

    RandomStack(QGraphicsItem *parent = nullptr, QUndoStack *undoStack  = nullptr);
    ~RandomStack();

    virtual void newGame() override;

    virtual bool canAdd(Card& card) const override;

    virtual void addCard(Card *card, bool flipTop) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual void fanCards(FanDirection dir) override;

protected slots:
    void fanAnimationFinished();
    void onUpdateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

protected:
};

#endif // CARDSTACK_H
