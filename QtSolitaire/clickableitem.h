#ifndef CLICKABLEGRAPHICSITEM_H
#define CLICKABLEGRAPHICSITEM_H

#include <QGraphicsTextItem>
/**
 * @brief The ClickableGraphicsTextItem class adds a clicked() signal to QGraphicsTextItem
 *
 * I looked at using the existing linkActivated symbol, but was having trouble getting that
 * working, and it also made the text look like a hyperlink, which I did not want.
 *
 */
class ClickableGraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    ClickableGraphicsTextItem() = delete;
    ClickableGraphicsTextItem(ClickableGraphicsTextItem &) = delete;
    ClickableGraphicsTextItem(ClickableGraphicsTextItem &&) = delete;
    ClickableGraphicsTextItem(QGraphicsItem *parent = nullptr);

signals:

    void clicked(ClickableGraphicsTextItem &item);

protected:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool mMouseDown;
};

#endif // CLICKABLEGRAPHICSITEM_H
