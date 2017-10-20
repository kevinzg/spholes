#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "View.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QGraphicsScene *scene;
    View *view;

    QToolBar *toolBar;

    QAction *setStartAction;
    QAction *setDestinationAction;

    void setupGraphicView();
    void setupActions();
    void setupToolBar();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setStart();
    void setDestination();
};

#endif // MAINWINDOW_H
