#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit Scene(QObject *parent = 0);

signals:
    void pointClicked(QPointF point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // SCENE_H
