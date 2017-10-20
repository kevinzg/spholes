#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "View.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QGraphicsScene *scene;
    View *view;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
