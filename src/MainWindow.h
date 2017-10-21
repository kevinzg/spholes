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

    void setupGraphicView();
    void setupGraphicItems();
    void setupActions();
    void setupToolBar();

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

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changeInteractionMode(QAction *);
    void scenePointClicked(QPointF);
};

#endif // MAINWINDOW_H
