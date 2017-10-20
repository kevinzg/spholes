#include "Scene.h"

#include <QGraphicsSceneMouseEvent>

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    emit pointClicked(mouseEvent->scenePos());
    QGraphicsScene::mousePressEvent(mouseEvent);
}
