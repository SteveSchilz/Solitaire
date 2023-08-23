#include "clickableitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

ClickableGraphicsTextItem::ClickableGraphicsTextItem(QGraphicsItem *parent)
    :QGraphicsTextItem(parent)
    ,mMouseDown(false)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setToolTip("Click Me!");
}

// Signal methods are created automatically in the moc output, do not define this method.
// void ClickableGraphicsTextItem::clicked() { }

void ClickableGraphicsTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mMouseDown = false;
    QGraphicsTextItem::hoverLeaveEvent(event);
}


void ClickableGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMouseDown = true;
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void ClickableGraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event);
}

void ClickableGraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (mMouseDown) {
        emit clicked(*this);
        event->accept();
        mMouseDown = false;
    } else {
        event->ignore();
    }
}
