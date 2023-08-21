#include "cardstack.h"
#include "constants.h"

#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

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
//    painter->save();
    if (mDragOver) {
        painter->setPen(Qt::black);
    } else  {
        painter->setPen(Qt::NoPen);
    }
    painter->setBrush(mColor);
    painter->drawRect(-(CARD_WIDTH/2+SHDW), -(CARD_HEIGHT/2+SHDW), CARD_WIDTH+SHDW, CARD_HEIGHT+SHDW);

//    QGraphicsRectItem::paint(painter, option, widget);
//    painter->restore();
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
