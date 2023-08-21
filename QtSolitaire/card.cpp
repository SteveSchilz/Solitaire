#include "card.h"
#include "constants.h"

#include <QApplication>
#include <QCursor>
#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QRandomGenerator>
#include <QSvgRenderer>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSvgItem>


Card::Card(CardValue v, Suite s, QGraphicsItem *parent)
    :mValue(v)
    ,mSuite(s)
    ,mImage{nullptr}
    ,mColor{Qt::red}
{
    if (mSuite == Suite::CLUB || mSuite == Suite::SPADE) {
        mColor = Qt::black;
    }
    setToolTip(QString("QColor(%1, %2, %3)\n%4")
                   .arg(mColor.red()).arg(mColor.green()).arg(mColor.blue())
                   .arg("Click and Drag card to play!"));

    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);

    const char *imgPath = getImagePath();
    if (imgPath != nullptr) {
        mImage = new QGraphicsSvgItem(getImagePath(), this);
        mImage->setScale(SVG_SCALEF);
        mImage->setTransformOriginPoint(QPointF(-CARD_WIDTH/2, -3-CARD_HEIGHT/2));
    }
    mPaintText = QString(getText()) + QString(getSuiteChar());

    if (debugLevel >= DEBUG_LEVEL::NORMAL) {
        qDebug() << "Created Card" << this;
    }
}

Card::~Card() {

    if (debugLevel >= DEBUG_LEVEL::NORMAL) {
        qDebug() << "Destroyed Card" << this;
    }
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
    if (debugLevel == DEBUG_LEVEL::VERBOSE && mValue == CardValue::JACK) {
        qDebug() << QString{"Painted Card %1 at {%2,%3}"}
                        .arg(mPaintText)
                        .arg(this->scenePos().rx()).arg(this->scenePos().ry());
    }

    // Draw Drop shadow
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawRect(-(CARD_WIDTH/2+SHDW), -(CARD_HEIGHT/2+SHDW), CARD_WIDTH+SHDW, CARD_HEIGHT+SHDW);
    // Draw square
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(-(CARD_WIDTH/2), -(CARD_HEIGHT/2), CARD_WIDTH, CARD_HEIGHT);
    if (mImage) {
        // NOTE: QGraphicsSvgItem has it's own paint method!
        //       Code below works, but performs a second paint on the image.
        //       We don't need to do anything here...
        painter->scale(SVG_SCALEF, SVG_SCALEF);
        painter->translate(QPointF(-(CARD_WIDTH/(SVG_SCALEF*2)),  -(CARD_HEIGHT/(SVG_SCALEF*2))));
        mImage->paint(painter, option, widget);

    } else {
        painter->setPen(mColor);
        // And the text
        painter->drawText(QPoint{-(CARD_WIDTH/2)+SHDW+1, -(CARD_WIDTH/2)+SHDW+1}, mPaintText);
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
    if (mImage) {
        QSvgRenderer *renderer = mImage->renderer();

        QImage image(CARD_WIDTH, CARD_HEIGHT, QImage::Format_ARGB32);
        image.fill(0xaaA08080);  // partly transparent red-ish background

        QPainter painter(&image);
        renderer->render(&painter);

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
        pixmap.fill(mColor);

        QPainter painter(&pixmap);
        painter.translate(CARD_WIDTH/2, CARD_HEIGHT/2);
        painter.setRenderHint(QPainter::Antialiasing);
        paint(&painter, nullptr, nullptr);
        painter.end();

        //Creates a mask where white is non-transparent.  Interesting
        //pixmap.setMask(pixmap.createMaskFromColor(Qt::white, Qt::MaskOutColor));

        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(CARD_WIDTH/2, CARD_HEIGHT/2));

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
