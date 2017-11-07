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
    this->setupGraphicItems();
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

    connect(scene, &Scene::pointClicked,
            this, &MainWindow::scenePointClicked);
}

void MainWindow::setupGraphicItems()
{
    startPoint = new PointItem;
    startPoint->setColor(Qt::blue);
    startPoint->setVisible(false);

    destinationPoint = new PointItem;
    destinationPoint->setColor(Qt::red);
    destinationPoint->setVisible(false);

    scene->addItem(startPoint);
    scene->addItem(destinationPoint);
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

void MainWindow::clearInteractionMode()
{
    actions->checkedAction()->setChecked(false);
    currentMode = NoInteraction;
}

void MainWindow::changeInteractionMode(QAction *action)
{
    currentMode = static_cast<InteractionMode>(action->data().value<int>());
}

void MainWindow::scenePointClicked(QPointF point)
{
    switch(currentMode)
    {
    case SetStart:
        startPoint->setPos(point);
        startPoint->setVisible(true);
        clearInteractionMode();
        break;
    case SetDestination:
        destinationPoint->setPos(point);
        destinationPoint->setVisible(true);
        clearInteractionMode();
        break;
    case NoInteraction:
    default:
        break;
    }
}
