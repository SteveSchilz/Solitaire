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
#include <QGraphicsSvgItem>


static const int CARD_WIDTH {60};
static const int CARD_HEIGHT {CARD_WIDTH*5/4};
static const int SHDW {3};  // Drop shadow offset

Card::Card(CardValue v, Suite s, QGraphicsItem *parent)
    :mValue(v)
    ,mSuite(s)
    ,mImage{nullptr}
{
    mColor = Qt::white;
    mTextColor = Qt::red;
    if (mSuite == Suite::CLUB || mSuite == Suite::SPADE) {
        mColor = Qt::white;
        mTextColor = Qt::black;
    }
    setToolTip(QString("QColor(%1, %2, %3)\n%4")
                   .arg(mColor.red()).arg(mColor.green()).arg(mColor.blue())
                   .arg("Click and Drag card to play!"));

    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);

    const char *imgPath = getImagePath();
    if (imgPath != nullptr) {
        mImage = new QGraphicsSvgItem(getImagePath(), this);
        mImage->setScale(0.08);
    }
    mPaintText = QString(getText()) + QString(getSuiteChar());

    qDebug() << "Created Card" << this;
}

Card::~Card() {
    qDebug() << "Destroyed Card" << this;

}

QRectF Card::boundingRect() const
{
    return QRectF(-(CARD_WIDTH/2), -(CARD_HEIGHT/2), CARD_WIDTH, CARD_HEIGHT);

}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    if (mImage) {
        painter->scale(.07, .07);
        painter->translate(QPointF(-(CARD_WIDTH/(.07*2)),  -(CARD_HEIGHT/(.07*2))));
        mImage->paint(painter, option, widget);

    } else {
        // Draw Drop shadow
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawRect(-(CARD_WIDTH/2+SHDW), -(CARD_HEIGHT/2+SHDW), CARD_WIDTH+SHDW, CARD_HEIGHT+SHDW);
        // Draw square
        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(QBrush(mColor));
        painter->drawRect(-(CARD_WIDTH/2), -(CARD_HEIGHT/2), CARD_WIDTH, CARD_HEIGHT);
        painter->setPen(mTextColor);
        // And the text
        painter->drawText(QPoint{-(CARD_WIDTH/2)+SHDW, -(CARD_WIDTH/2)+SHDW}, mPaintText);
    }
    painter->restore();
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

        drag->setPixmap(QPixmap::fromImage(image).scaled(CARD_WIDTH, CARD_HEIGHT));
        drag->setHotSpot(QPoint(CARD_WIDTH/2,CARD_HEIGHT/2));

    } else {
        mime->setColorData(mColor);
        mime->setText(QString("#%1%2%3")
                          .arg(mColor.red(), 2, 16, QLatin1Char('0'))
                          .arg(mColor.green(), 2, 16, QLatin1Char('0'))
                          .arg(mColor.blue(), 2, 16, QLatin1Char('0')));

        QPixmap pixmap(CARD_WIDTH, CARD_HEIGHT);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.translate(CARD_WIDTH/2, CARD_HEIGHT/2);
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

QChar Card::getSuiteChar() {

    switch(mSuite) {
    case Suite::HEART: return QChar::fromUcs2(0x2665); break;
    case Suite::DIAMOND: return QChar::fromUcs2(0x2666); break;
    case Suite::CLUB: return QChar::fromUcs2(0x2663);
    case Suite::SPADE: return QChar::fromUcs2(0x2660);
    }
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

const char *Card::getImagePath() {
    switch (mSuite) {
    case Suite::HEART:
        switch(mValue) {
        case CardValue::KING:  return ":/images/King-Hearts.svg"; break;
        case CardValue::QUEEN: return ":/images/Queen-Hearts.svg"; break;
        case CardValue::JACK:  return ":/images/Jack-Hearts.svg"; break;
        default:    return nullptr;
        }
        break;

    case Suite::DIAMOND:
        switch(mValue) {
        case CardValue::KING:  return ":/images/King-Diamonds.svg"; break;
        case CardValue::QUEEN: return ":/images/Queen-Diamonds.svg"; break;
        case CardValue::JACK:  return ":/images/Jack-Diamonds.svg"; break;
        default:    return nullptr;
        }
        break;
    case Suite::CLUB:
        switch(mValue) {
        case CardValue::KING:  return ":/images/King-Clubs.svg"; break;
        case CardValue::QUEEN: return ":/images/Queen-Clubs.svg"; break;
        case CardValue::JACK:  return ":/images/Jack-Clubs.svg"; break;
        default:    return nullptr;
        }
        break;
    case Suite::SPADE:
        switch(mValue) {
        case CardValue::KING:  return ":/images/King-Spades.svg"; break;
        case CardValue::QUEEN: return ":/images/Queen-Spades.svg"; break;
        case CardValue::JACK:  return ":/images/Jack-Spades.svg"; break;
        default:    return nullptr;
        }
        break;
    }

}
