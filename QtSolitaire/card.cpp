#include "card.h"

#include <QApplication>
#include <QCursor>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QRandomGenerator>
#include <QWidget>
#include <QDebug>

static const int CARD_WIDTH {120};
static const int CARD_HEIGHT {CARD_WIDTH*5/4};
Card::Card(CardValue v, Suite s, QGraphicsItem *parent)
    :mValue(v)
    ,mSuite(s)
    ,mImage{nullptr}
{
    mColor = Qt::red;
    mTextColor = Qt::black;
    if (mSuite == Suite::CLUB || mSuite == Suite::SPADE) {
        mColor = Qt::black;
        mTextColor = Qt::white;
    }
    setToolTip(QString("QColor(%1, %2, %3)\n%4")
                   .arg(mColor.red()).arg(mColor.green()).arg(mColor.blue())
                   .arg("Click and Drag card to play!"));

    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
    qDebug() << "Created Card" << this;
}

Card::~Card() {
    qDebug() << "Destroyed Card" << this;

}

void Card::setPixmap(QString path) {

    mImage = new QPixmap(path);
}


QRectF Card::boundingRect() const
{
//    return QRectF(-(CARD_WIDTH/2), -(CARD_HEIGHT/2), CARD_WIDTH, CARD_HEIGHT);
    return QRectF(-15.5, -15.5, 34, 34);
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (mImage && !mImage->isNull()) {
        painter->scale(.04, .04);
        painter->drawPixmap(QPointF(-15 * 4.4, -50 * 3.54), *mImage);

    } else {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawEllipse(-15, -15, 30, 30);
        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(QBrush(mColor));
        painter->drawEllipse(-15, -15, 30, 30);
    }
    painter->setPen(mTextColor);
    painter->drawText(QPoint{-4, +4}, getText());

}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::ClosedHandCursor);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);

    static int n = 0;
    if (n++ > 2 && QRandomGenerator::global()->bounded(3) == 0) {
        QImage image(":/images/King-Diamond.png");
        mime->setImageData(image);

        drag->setPixmap(QPixmap::fromImage(image).scaled(30, 40));
        drag->setHotSpot(QPoint(15, 30));

    } else {
        mime->setColorData(mColor);
        mime->setText(QString("#%1%2%3")
                          .arg(mColor.red(), 2, 16, QLatin1Char('0'))
                          .arg(mColor.green(), 2, 16, QLatin1Char('0'))
                          .arg(mColor.blue(), 2, 16, QLatin1Char('0')));

        QPixmap pixmap(34, 34);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.translate(15, 15);
        painter.setRenderHint(QPainter::Antialiasing);
        paint(&painter, nullptr, nullptr);
        painter.end();

        pixmap.setMask(pixmap.createHeuristicMask());

        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(15, 20));

    }

    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);
}

const char *Card::getText() {

    switch(mValue) {
        case CardValue::ACE: return "A"; break;
        case CardValue::TWO: return "2"; break;
        case CardValue::THREE: return "3"; break;
        case CardValue::FOUR: return "4"; break;
        case CardValue::FIVE: return "5"; break;
        case CardValue::SIX: return "6"; break;
        case CardValue::SEVEN: return "7"; break;
        case CardValue::EIGHT: return "8"; break;
        case CardValue::NINE: return "9"; break;
        case CardValue::TEN: return "10"; break;
        case CardValue::JACK: return "J"; break;
        case CardValue::QUEEN: return "Q"; break;
        case CardValue::KING: return "K"; break;
    }

}
