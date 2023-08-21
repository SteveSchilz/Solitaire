#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

/*
 * Debug Class to show coordinates from scene when left mouse button pressed.
 */
class myScene: public QGraphicsScene {
public:
    myScene() = delete;
    myScene(qreal x, qreal y, qreal width, qreal height, QObject* parent = nullptr)
        :QGraphicsScene(x,y,width,height,parent)
    {

    }
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        qDebug() << QString("Pos (%1,%2)").arg(event->scenePos().rx()).arg(event->scenePos().ry());
        QGraphicsScene::mousePressEvent(event);
    }

};

#endif // MYSCENE_H
