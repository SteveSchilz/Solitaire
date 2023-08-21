#ifndef CARDSTACK_H
#define CARDSTACK_H

#include <QObject>
#include <QGraphicsItem>

class CardStack : public QGraphicsItem
{
public:
    Q_DISABLE_COPY(CardStack)

    CardStack() = delete;
    CardStack(QGraphicsItem *parent = nullptr);
    ~CardStack();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;


private:
    QColor mColor = Qt::lightGray;
    bool mDragOver = false;
};

#endif // CARDSTACK_H
