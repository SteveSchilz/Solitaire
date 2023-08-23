#include "cardstack.h"
#include "constants.h"

#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSvgItem>
#include <QIoDevice>
#include <QMimeData>

/******************************************************************************
 * CardStack Implementation
 *****************************************************************************/
CardStack::CardStack(QGraphicsItem *parent)
    :QGraphicsItem{parent}
    ,mColor{Qt::lightGray}
    ,mDragOver{false}
{
    setToolTip(QString("Drop Matching Cards Here\n"));

    setAcceptDrops(true);

    if (debugLevel >= DEBUG_LEVEL::NORMAL) {
        qDebug() << "Created CardStack" << this;
    }
}

CardStack::~CardStack() {

    if (debugLevel >= DEBUG_LEVEL::NORMAL) {
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
    , mDropAccepted(false)
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
    if (!mDropAccepted) {
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
        // TODO: ugly casting to get next value of class enum!
        CardValue testValue = mCards.size() == 0 ? CardValue::ACE : static_cast<CardValue>((int)mCards.top()->getValue() +1);
        if (droppedCard && droppedCard->getSuite() == mSuite) {
            if((droppedCard->getValue() == testValue) ||
                testValue == droppedCard->getValue()) {
                mDropAccepted = true;
                mImage->setVisible(false);
                droppedCard->setPos(QPoint(0,0));

                droppedCard->setParentItem(this);
                mCards.push(droppedCard);
            }
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
 * RandomStack Implementation
 *****************************************************************************/
RandomStack::RandomStack(QGraphicsItem *parent)
    : CardStack(parent)
    , mDropAccepted(false)
{
    const char *imgPath = getImagePath();
    if (imgPath != nullptr) {
        mImage = new QGraphicsSvgItem(imgPath, this);
        mImage->setScale(SVG_SCALEF);
        mImage->setOpacity(0.4);
        mImage->setPos(-CARD_WIDTH/2.0, -CARD_HEIGHT/2.0);
    }

}

RandomStack::~RandomStack()
{

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
    if (!mDropAccepted) {
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);
    } else {
        CardStack::paint(painter, option, widget);
    }
    painter->restore();
}

void RandomStack::addCard(Card& card)
{
    mCards.append(&card);
    card.setParentItem(this);
    card.setPos(0,0);
}


const char *RandomStack::getImagePath()
{
    return ":/images/Card-Back.svg";
}
