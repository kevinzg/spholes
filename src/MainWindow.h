#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRectF>
#include "View.h"
#include "Scene.h"
#include "PointItem.h"
#include "spholes/Path.h"

class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    static const qreal SCENE_BOX_RADIUS;
    static const QRectF SCENE_RECT;

private:
    Scene *scene;
    View *view;

    QActionGroup *actions;
    QAction *stopAction;
    QAction *cancelAction;
    QAction *solveAction;

    void setupGraphicView();
    void setupGraphicItems();
    void setupActions();
    void setupToolBar();

    void clearInteractionMode();
    void addPointToNewPolygon(QPointF);

    void drawShortestPath(const spholes::Path &);

    enum InteractionMode
    {
        NoInteraction,
        SetStart,
        SetDestination,
        AddNewPolygon
    };

    InteractionMode currentMode;

    PointItem *startPoint;
    PointItem *destinationPoint;

    QGraphicsItemGroup *obstacleGroup;

    QGraphicsItemGroup *newPolygon;
    PointItem *newPolygonStartPoint;
    QGraphicsPathItem *newPolygonPath;

    QGraphicsPathItem *shortestPath;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changeInteractionMode(QAction *);
    void stopActionTriggered();
    void cancelActionTriggered();
    void solveActionTriggered();
    void scenePointClicked(QPointF);
};

#endif // MAINWINDOW_H
