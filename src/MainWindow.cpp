#include "MainWindow.h"
#include "View.h"

#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setupGraphicView();
    this->setupActions();
    this->setupToolBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupGraphicView()
{
    scene = new QGraphicsScene(this);

    view = new View(tr("Euclidean Shortest Path"), this);
    view->view()->setScene(scene);
    this->setCentralWidget(view);
}

void MainWindow::setupActions()
{
    setStartAction = new QAction(tr("Set Start Point"), this);
    connect(setStartAction, SIGNAL(triggered()), this, SLOT(setStart()));

    setDestinationAction = new QAction(tr("Set Destination Point"), this);
    connect(setDestinationAction, SIGNAL(triggered()), this, SLOT(setDestination()));
}

void MainWindow::setupToolBar()
{
    toolBar = new QToolBar;
    this->addToolBar(toolBar);

    toolBar->addAction(setStartAction);
    toolBar->addAction(setDestinationAction);
}

void MainWindow::setStart()
{
}

void MainWindow::setDestination()
{
}
