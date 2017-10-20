#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "View.h"

class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QGraphicsScene *scene;
    View *view;

    QActionGroup *actions;

    void setupGraphicView();
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

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void changeInteractionMode(QAction *);
};

#endif // MAINWINDOW_H
