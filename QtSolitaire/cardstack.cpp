#include "cardstack.h"
#include "constants.h"

#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSvgItem>
#include <QIoDevice>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

const int FAN_DURATION_MS{750};             ///< Time in ms for Card Fanning animation to run

/**
 * @brief getFanLocation calculates x/y values for a stack of cards that is fanned out across the table
 *
 * @param i index of card in stack
 * @param n number of cards to be fanned out
 * @param f FanDirection (Horizontal, Vertical, Other?)
 * @param fanRegion - the area to fan the cards across in pixels is specified by a Qrect: which is two points(topLeft, bottom right)
 *
 * @return Point in scene coordinates for the specified card
 */
QPointF getFanLocation(int i, int n, FanDirection f, QRect fanRegion ) {
    double x{0.0}, y{0.0};

    // Todo: Testing: check right >= left, bottom >= top (same acceptable)
    int xDistance = (fanRegion.right() - fanRegion.left())/n;
    int yDistance = (fanRegion.bottom() - fanRegion.top())/n;

    switch (f) {
    case FanDirection::FOUR_ROWS:
        x = (double)(i % 13) * 30.0;
        y = (double)(i / 13) * 30.0;
        break;
    case FanDirection::HORIZONTAL:
        x = (double)(fanRegion.left() + i*xDistance);
        y = (double)(fanRegion.top());
        break;
    case FanDirection::VERTICAL:
        x = (double)(fanRegion.left());
        y = (double)(fanRegion.top() + i*yDistance);
        break;

    }
    return QPointF(x, y);
}

/******************************************************************************
 * CardStack Implementation
 *****************************************************************************/
CardStack::CardStack(QGraphicsItem *parent)
    :QGraphicsObject{parent}
    ,mColor{Qt::lightGray}
    ,mDragOver{false}
{
    setToolTip(QString("Drop Matching Cards Here\n"));

    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptDrops(true);

    if (debugLevel >= DEBUG_LEVEL::VERBOSE) {
        qDebug() << "Created CardStack" << this;
    }
}

CardStack::~CardStack() {

    if (debugLevel >= DEBUG_LEVEL::VERBOSE) {
        qDebug() << "Destroyed CardStack" << this;
    }
}


QRectF CardStack::boundingRect() const
{
    return QRectF(-(CARD_WIDTH/2), -(CARD_HEIGHT/2), CARD_WIDTH, CARD_HEIGHT);
}

void CardStack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    if (mDragOver) {
        painter->setPen(Qt::black);
    } else  {
        painter->setPen(Qt::NoPen);
    }
    painter->setBrush(mColor);
    painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);
    painter->restore();
}

void CardStack::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mMouseDown = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void CardStack::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMouseDown = true;
        event->accept();
        setCursor(Qt::ClosedHandCursor);
    }
    else
    {
        event->ignore();
    }
}

void CardStack::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (mMouseDown) {
        mMouseDown = false;
        emit clicked(*this);
    }
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);

}


void CardStack::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasColor()) {
        event->setAccepted(true);
        mDragOver = true;
        update();
    } else {
        event->setAccepted(false);
    }
}

void CardStack::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
    mDragOver = false;
    update();
}
void CardStack::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    mDragOver = false;
    if (event->mimeData()->hasColor())
        mColor = qvariant_cast<QColor>(event->mimeData()->colorData());
    update();
}

/******************************************************************************
 * SortedStack Implementation
 *****************************************************************************/
SortedStack::SortedStack(Suite s, QGraphicsItem *parent)
    : CardStack(parent)
    , mSuite{s}
{
    const char *imgPath = getImagePath(s);
    if (imgPath != nullptr) {
        mImage = new QGraphicsSvgItem(imgPath, this);
        mImage->setScale(0.2);
        mImage->setOpacity(0.4);
        mImage->setPos(-CARD_WIDTH/4.0, -CARD_HEIGHT/4.0);
    }

}

SortedStack::~SortedStack()
{

}

bool SortedStack::canAdd(Card& card) const
{
    // TODO: ugly casting to get next value of class enum!
    CardValue testValue = mCards.size() == 0 ? CardValue::ACE : static_cast<CardValue>((int)mCards.top()->getValue() +1);

    return ((mSuite == card.getSuite()) &&
            (testValue == card.getValue()));
}

void SortedStack::fanCards(FanDirection dir)
{
    QSizeF newLocation{0.0, 0.0};
    QPropertyAnimation* a;
    QParallelAnimationGroup aGroup;

    int i {0};
    int numCards = mCards.size()+1;

    for (auto it : mCards) {
        a = new QPropertyAnimation(it, "pos");
        a->setDuration(FAN_DURATION_MS);
        a->setEasingCurve(QEasingCurve::InOutBack);
        a->setStartValue(it->pos());
        a->setEndValue(getFanLocation(i, numCards, FanDirection::HORIZONTAL, QRect(100,20, 700,80)));
        aGroup.addAnimation(a);
        i++;
    }

    QObject::connect(&aGroup, &QAbstractAnimation::finished, this, &SortedStack::fanAnimationFinished);
    aGroup.start(QAbstractAnimation::DeleteWhenStopped);

}

void SortedStack::fanAnimationFinished() {

    int i = 0;
    Card *prevCard = nullptr;

    for (auto it : mCards) {
        if (prevCard) {
            it->stackBefore(prevCard);
        }
        prevCard = it;
    }
    update();
}

void SortedStack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    if (mDragOver) {
        painter->setPen(Qt::black);
    } else  {
        painter->setPen(Qt::lightGray);
    }
    if (mCards.size() == 0) {
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);
    } else {
        CardStack::paint(painter, option, widget);
    }
    painter->restore();
}

void SortedStack::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    Card *droppedCard{nullptr};

    mDragOver = false;

    if(event->mimeData()->hasFormat(CARD_MIME_TYPE)) {
        QByteArray itemData = event->mimeData()->data("application/x-card");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        dataStream >> droppedCard;
        if (droppedCard && canAdd(*droppedCard)) {
                mImage->setVisible(false);
                droppedCard->setPos(QPoint(0,0));

                droppedCard->setParentItem(this);
                mCards.push(droppedCard);
                event->setAccepted(true);
                update();
            }
        } else {
            event->setAccepted(false);
        }
}

const char *SortedStack::getImagePath(Suite s)
{
    switch(s) {
    case Suite::HEART: return ":/images/Heart.svg"; break;
    case Suite::DIAMOND: return ":/images/Diamond.svg"; break;
    case Suite::SPADE: return ":/images/Spade.svg"; break;
    case Suite::CLUB: return ":/images/Club.svg"; break;
    default:    return nullptr;
    }
}


/******************************************************************************
 * DescendingStack Implementation
 *****************************************************************************/
DescendingStack::DescendingStack(QGraphicsItem *parent)
    : CardStack(parent)
{

}

DescendingStack::~DescendingStack()
{

}

bool DescendingStack::canAdd(Card& card) const
{
    CardValue testValue = CardValue::KING;      // Empty stack will allow you to drop a king
    QColor testColor = Qt::white;               // Use a non-valid color to allow drops when empty

    // TODO: ugly casting to get next value of class enum!
    if (!mCards.isEmpty()) {
            if (mCards.top()->getValue() == CardValue::ACE) {
                return false;
            } else {
                testValue = static_cast<CardValue>((int)mCards.top()->getValue() - 1);
                testColor = mCards.top()->getColor();
            }
    }
    return ((testValue == card.getValue()) &&
            (testColor != card.getColor()));
}


QRectF DescendingStack::boundingRect() const
{
    double yAddress = getYOffset();
    return QRectF(-(CARD_WIDTH/2), yAddress-(CARD_HEIGHT/2), CARD_WIDTH, yAddress+CARD_HEIGHT);
}


void DescendingStack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    if (mDragOver) {
        painter->setPen(Qt::black);
    } else if (mCards.size() == 0) {
        painter->setPen(Qt::lightGray);
    } else {
        painter->setPen(Qt::NoPen);
    }
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);
    painter->restore();
}

void DescendingStack::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    Card *droppedCard{nullptr};
    double yAddress{0.0};

    mDragOver = false;

    if(event->mimeData()->hasFormat(CARD_MIME_TYPE)) {
        QByteArray itemData = event->mimeData()->data("application/x-card");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        dataStream >> droppedCard;
        if (!droppedCard) {
                event->setAccepted(false);
                return;
        }
        if (canAdd(*droppedCard))  {
            QGraphicsItem *otherStack = droppedCard->parentItem();

            // TODO: This ugly stuff requires friend class definitions and removes the cards from the drag source stack
            //       perhaps there is a way to remove them from the stack when the drag
            //       starts and then undo that if the drag is canceled?
            //       Might require serializing the cards onto the mime data??
            DescendingStack *dStack = dynamic_cast<DescendingStack*>(otherStack);
            if (dStack && !dStack->mCards.empty() && dStack->mCards.back() == droppedCard) {
                qDebug() << "taking from DescendingStack back";
                dStack->mCards.pop_back();
            }
            if (dStack && !dStack->mCards.empty() && dStack->mCards.front() == droppedCard) {
                qDebug() << "taking from DescendingStack front";
                dStack->mCards.pop_front();
            }
            RandomStack *rStack = dynamic_cast<RandomStack*>(otherStack);
            if (rStack && !rStack->mCards.empty() && rStack->mCards.back() == droppedCard) {
                qDebug() << "taking from RandomStack back";
                rStack->mCards.pop_back();
            }
            if (rStack && !rStack->mCards.empty() && rStack->mCards.front() == droppedCard) {
                qDebug() << "taking from RandomStack frontk";
                rStack->mCards.pop_front();
            }

            SortedStack *sStack = dynamic_cast<SortedStack*>(otherStack);
            if (sStack && !sStack->mCards.empty() && sStack->mCards.back() == droppedCard) {
                qDebug() << "taking from SortedStack back";
                sStack->mCards.pop_back();
            }
            if (sStack && !sStack->mCards.empty() && sStack->mCards.front() == droppedCard) {
                qDebug() << "taking from SortedStack frontk";
                sStack->mCards.pop_front();
            }

            mCards.push(droppedCard);
            droppedCard->setPos(QPointF(0.0, this->getYOffset()));
            droppedCard->setParentItem(this);
            update();
        } else {
            event->setAccepted(false);
        }
    } else {
            event->setAccepted(false);
        }
}

void DescendingStack::addCard(Card* card)
{
        if (card) {
            mCards.push(card);
            card->setParentItem(this);
            card->setPos(0,0);
        }
}

double DescendingStack::getYOffset() const
{
        double yAddress = 0.0;
        if (mCards.size() > 0) {
            yAddress = ((double)mCards.size()-1)*15.0;
        }
        return yAddress;
}

void DescendingStack::fanCards(FanDirection dir)
{
        QSizeF newLocation{0.0, 0.0};
        QPropertyAnimation* a;
        QParallelAnimationGroup aGroup;

        int i {0};
        int numCards = mCards.size()+1;

        for (auto it : mCards) {
            a = new QPropertyAnimation(it, "pos");
            a->setDuration(FAN_DURATION_MS);
            a->setEasingCurve(QEasingCurve::InOutBack);
            a->setStartValue(it->pos());
            a->setEndValue(getFanLocation(i, numCards, FanDirection::HORIZONTAL, QRect(100,20, 700,80)));
            aGroup.addAnimation(a);
            i++;
        }

        QObject::connect(&aGroup, &QAbstractAnimation::finished, this, &DescendingStack::fanAnimationFinished);
        aGroup.start(QAbstractAnimation::DeleteWhenStopped);

}

void DescendingStack::fanAnimationFinished() {

        int i = 0;
        Card *prevCard = nullptr;

        for (auto it : mCards) {
            if (prevCard) {
            it->stackBefore(prevCard);
            }
            prevCard = it;
        }
        update();
}


void DescendingStack::onUpdateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
        qDebug() << "New State:" << newState;
        if (oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::Stopped) {
            this->fanAnimationFinished();
        }
}



/******************************************************************************
 * RandomStack Implementation
 *****************************************************************************/
RandomStack::RandomStack(QGraphicsItem *parent)
    : CardStack(parent)
{
}

RandomStack::~RandomStack()
{

}

bool RandomStack::canAdd(Card& card) const
{
    return false;
}

//TODO: QParallelAnimationGroup is not working!  WTH!
static bool useGroup = false;

void RandomStack::fanCards(FanDirection dir)
{
    QPropertyAnimation* a;
    QParallelAnimationGroup aGroup{this};
    QSizeF newLocation{0.0, 0.0};
    int i {0};
    int numCards = mCards.size()+1;

    for (auto it : mCards) {
        a = new QPropertyAnimation(it, "pos");
        a->setDuration(FAN_DURATION_MS);
        a->setEasingCurve(QEasingCurve::InOutBack);
        a->setStartValue(it->pos());
        a->setEndValue(getFanLocation(i, numCards, dir, QRect(20,20, 600,80)));
        if (useGroup)
            aGroup.addAnimation(a);
        else
            a->start();

        i++;
    }

    // BUG: ?? Connect succeeds for the finished slot, but the slot is not called, had to workaround using statechange.
    //      Possible OSX only problem? Check on windows
    //bool result = QObject::connect(&aGroup, &QAbstractAnimation::finished, this, &RandomStack::fanAnimationFinished);

    bool result2 = QObject::connect(&aGroup, &QAbstractAnimation::stateChanged, this, &RandomStack::onUpdateState);
    qDebug() << "Animation" ""<< (useGroup ?  "Group" : "noGroup") << "Connected:" << result2 << "Duration: " << aGroup.duration();
    if (useGroup)
        aGroup.start(QAbstractAnimation::DeleteWhenStopped);
}

void RandomStack::onUpdateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    qDebug() << "New State:" << newState;
    if (oldState == QAbstractAnimation::Running && newState == QAbstractAnimation::Stopped) {
        this->fanAnimationFinished();
    }
}

/**
 * @brief RandomStack::fanAnimationFinished - Attempt to reorder the cards after shuffling
 *
 * The fan animation lays out the cards based on their position in the deck.
 * After shuffling the deck, the cards are currently not stacked in order,
 * so the appearance is odd. This is an attempt to get them to stack in order,
 * but is not currently working.
 *
 * Algorithm variable tries a bunch of different stuff to make it happen. No dice.
 */

static int algorithm = 0;

void RandomStack::fanAnimationFinished() {

    Card *prevCard = nullptr;
    int i = 0;

    for (auto it : mCards) {
        if (prevCard) {
            if (algorithm % 3 == 0) {
                it->setPos(it->pos());
            } else if (algorithm % 3 == 1) {
                it->setZValue(i);
            } else if (algorithm % 3 == 2) {
                it->stackBefore(prevCard);
            }
            i++;
            it->hide();
            it->update();
        }
        prevCard = it;
    }
    qDebug() << "Fanned cards using algorithm " << algorithm++;

    for (auto it: mCards) {
        it->show();
    }
    update();
}

void RandomStack::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    if (mDragOver) {
        painter->setPen(Qt::black);
    } else  {
        painter->setPen(Qt::lightGray);
    }
    if (mCards.size() == 0) {
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);
    } else {
        CardStack::paint(painter, option, widget);
    }
    painter->restore();
}

void RandomStack::addCard(Card* card)
{
    if (card) {
        mCards.append(card);
    //    card->setFaceUp(true);
        card->setParentItem(this);
        card->setPos(0,0);
    }
}

Card *RandomStack::takeCard() {
    Card* result{nullptr};
    if (!mCards.isEmpty()) {
        result = mCards.takeLast();
    }
    return result;
}
