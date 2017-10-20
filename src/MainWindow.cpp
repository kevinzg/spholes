#include "MainWindow.h"
#include "View.h"

#include <QToolBar>
#include <QAction>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      currentMode(InteractionMode::NoInteraction)
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
    scene = new Scene(this);

    view = new View(tr("Euclidean Shortest Path"), this);
    view->view()->setScene(scene);
    this->setCentralWidget(view);
}

void MainWindow::setupActions()
{
    this->actions = new QActionGroup(this);

    QAction *setStartAction = new QAction(tr("Set Start Point"), this);
    setStartAction->setCheckable(true);
    setStartAction->setData(QVariant(InteractionMode::SetStart));

    QAction *setDestinationAction = new QAction(tr("Set Destination Point"), this);
    setDestinationAction->setCheckable(true);
    setDestinationAction->setData(QVariant(InteractionMode::SetDestination));

    actions->addAction(setStartAction);
    actions->addAction(setDestinationAction);

    connect(actions, &QActionGroup::triggered,
            this, &MainWindow::changeInteractionMode);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = new QToolBar(this);
    this->addToolBar(toolBar);

    toolBar->addActions(actions->actions());
}

void MainWindow::changeInteractionMode(QAction *action)
{
    currentMode = static_cast<InteractionMode>(action->data().value<int>());
}
