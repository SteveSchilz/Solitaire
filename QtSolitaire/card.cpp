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

/******************************************************************************
  * Card Implementation
  *****************************************************************************/
/**
  * @brief Ctor
  * @param v Card Value Ace -> King
  * @param s Sute (Heart, Diamond, Club, Spade)
  * @param parent object
  */
Card::Card(CardValue v, Suite s, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,mFaceUp(true)
    ,mMouseDown(false)
    ,mValue(v)
    ,mSuite(s)
    ,mImage{nullptr}
    ,mBackImage{nullptr}
    ,mColor{Qt::red}
{
    if (mSuite == Suite::CLUB || mSuite == Suite::SPADE) {
        mColor = Qt::black;
    }
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);

    const char *imgPath = getImagePath();
    if (imgPath != nullptr) {
        mImage = new QGraphicsSvgItem(getImagePath(), this);
        mImage->setScale(SVG_SCALEF);
        mImage->setTransformOriginPoint(QPointF(-CARD_WIDTH/2, -3-CARD_HEIGHT/2));
        mImage->setVisible(true);
    }

    mBackImage = new QGraphicsSvgItem(":/images/Card-Back.svg", this);
    mBackImage->setScale(SVG_SCALEF);
    mBackImage->setTransformOriginPoint(QPointF(-CARD_WIDTH/2, -3-CARD_HEIGHT/2));
    mBackImage->setVisible(false);

    mPaintText = QString(getValueText()) + QString(getSuiteChar());

    setToolTip(QString("%1").arg(mPaintText));

    // Hacky debug level, static cast became necessary when we added signal to this
    // class, making the "this" pointer ambigous without it.
    if (debugLevel >= DEBUG_LEVEL::VERBOSE) {
        qDebug() << "Created Card" <<  static_cast<QGraphicsItem*>(this);
    }
}

Card::~Card() {

    if (debugLevel >= DEBUG_LEVEL::VERBOSE) {
        qDebug() << "Destroyed Card" << static_cast<QGraphicsItem*>(this);
    }
}

void Card::setFaceUp(bool faceUp)
{
    if (faceUp != mFaceUp) {
        mFaceUp = faceUp;
        if (mImage) {
            mImage->setVisible(faceUp);
        }
        if (mBackImage) {
            mBackImage->setVisible(!faceUp);
        }
        update();
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

    if (mImage) {
        // NOTE: QGraphicsSvgItem has it's own paint method!
        //       Code below works, but performs a second paint on the image.
        //       We don't need to do anything here...
    //    painter->scale(SVG_SCALEF, SVG_SCALEF);
    //    painter->translate(QPointF(-(CARD_WIDTH/(SVG_SCALEF*2)),  -(CARD_HEIGHT/(SVG_SCALEF*2))));
    //    mImage->paint(painter, option, widget);

    } else {
        // Draw Drop shadow
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);

        // Draw square
        painter->setPen(QPen(Qt::black, 1));
        painter->setBrush(QBrush(Qt::white));
        painter->drawRoundedRect(boundingRect(), CARD_RADIUS, CARD_RADIUS);

        // And the text
        painter->setPen(mColor);
        painter->drawText(QPoint{-(CARD_WIDTH/2)+SHDW+1, -(CARD_WIDTH/2)+SHDW+1}, mPaintText);
    }
    painter->restore();
}

// Signal definitions appear in the moc output, no need to define them.
// void Card::clicked(Card& card) { }

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
// Docs say default impl handles selection and moving...
//    QGraphicsItem::mousePressEvent(event);
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

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);

    static int n = 0;
    mime->setText(mPaintText);
    mime->setColorData(mColor);

    // Serialize a pointer to the card into the mime data
    QByteArray itemData;
    QDataStream dataStream{&itemData, QIODevice::WriteOnly};
    dataStream << this;

    mime->setData(CARD_MIME_TYPE, itemData);

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

        QPixmap pixmap(CARD_WIDTH, CARD_HEIGHT);
        pixmap.fill(mColor);

        QPainter painter(&pixmap);
        painter.translate(CARD_WIDTH/2, CARD_HEIGHT/2);
        painter.setRenderHint(QPainter::Antialiasing);
        paint(&painter, nullptr, nullptr);
        painter.end();

        //Creates a mask where white is non-transparent.  Interesting
        pixmap.setMask(pixmap.createMaskFromColor(Qt::white, Qt::MaskOutColor));

        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(CARD_WIDTH/2, CARD_HEIGHT/2));

    }

    drag->exec(Qt::MoveAction);
    setCursor(Qt::OpenHandCursor);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mMouseDown = false;
    QGraphicsItem::hoverLeaveEvent(event);
}


void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (mMouseDown) {
        mMouseDown = false;
        emit clicked(*this);
    }
    setCursor(Qt::OpenHandCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}

QChar Card::getSuiteChar() {

    switch(mSuite) {
    case Suite::HEART: return QChar::fromUcs2(0x2665); break;
    case Suite::DIAMOND: return QChar::fromUcs2(0x2666); break;
    case Suite::CLUB: return QChar::fromUcs2(0x2663);
    case Suite::SPADE: return QChar::fromUcs2(0x2660);
    }
}


const char *Card::getValueText() {

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

QDataStream & operator << (QDataStream & s, const Card *cardptr)
{
    /** WARNING: non-portable code **/
    qulonglong ptrval(*reinterpret_cast<qulonglong *>(&cardptr));
    return s << ptrval;
}
QDataStream & operator >> (QDataStream & s, Card *& cardptr)
{
    qulonglong ptrval;
    s >> ptrval;
    cardptr = *reinterpret_cast<Card **>(&ptrval);
    return s;
}

