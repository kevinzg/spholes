#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "View.h"
#include "Scene.h"
#include "PointItem.h"

class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Scene *scene;
    View *view;

    QActionGroup *actions;
    QAction *stopAction;
    QAction *cancelAction;

    void setupGraphicView();
    void setupGraphicItems();
    void setupActions();
    void setupToolBar();

    void clearInteractionMode();
    void addPointToNewPolygon(QPointF);

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

    QGraphicsItemGroup *newPolygon;
    PointItem *newPolygonStartPoint;
    QGraphicsPathItem *newPolygonPath;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changeInteractionMode(QAction *);
    void stopActionTriggered();
    void cancelActionTriggered();
    void scenePointClicked(QPointF);
};

#endif // MAINWINDOW_H
