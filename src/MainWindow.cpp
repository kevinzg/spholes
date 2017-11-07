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

    newPolygon = new QGraphicsItemGroup;
    newPolygon->setVisible(false);

    newPolygonStartPoint = new PointItem;
    newPolygonStartPoint->setColor(Qt::black);

    newPolygonPath = new QGraphicsPathItem;
    newPolygonPath->setPen(QPen(Qt::black));

    newPolygon->addToGroup(newPolygonStartPoint);
    newPolygon->addToGroup(newPolygonPath);

    scene->addItem(startPoint);
    scene->addItem(destinationPoint);
    scene->addItem(newPolygon);
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

    QAction *addNewPolygonAction = new QAction(tr("Add New Polygon"), this);
    addNewPolygonAction->setCheckable(true);
    addNewPolygonAction->setData(QVariant(InteractionMode::AddNewPolygon));

    actions->addAction(setStartAction);
    actions->addAction(setDestinationAction);
    actions->addAction(addNewPolygonAction);

    connect(actions, &QActionGroup::triggered,
            this, &MainWindow::changeInteractionMode);

    stopAction = new QAction(tr("Close Polygon"), this);
    stopAction->setEnabled(false);

    connect(stopAction, &QAction::triggered,
            this, &MainWindow::stopActionTriggered);

    cancelAction = new QAction(tr("Cancel"), this);
    cancelAction->setEnabled(false);

    connect(cancelAction, &QAction::triggered,
            this, &MainWindow::cancelActionTriggered);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = new QToolBar(this);
    this->addToolBar(toolBar);

    toolBar->addActions(actions->actions());
    toolBar->addAction(stopAction);
    toolBar->addAction(cancelAction);
}

void MainWindow::clearInteractionMode()
{
    actions->checkedAction()->setChecked(false);
    actions->setEnabled(true);
    stopAction->setEnabled(false);
    cancelAction->setEnabled(false);

    newPolygon->setVisible(false);

    currentMode = NoInteraction;
}

void MainWindow::addPointToNewPolygon(QPointF point)
{
    QPainterPath polygonPath = newPolygonPath->path();

    if (polygonPath.elementCount() == 0)
    {
        newPolygonStartPoint->setPos(point);
        polygonPath.moveTo(point);
    }
    else
        polygonPath.lineTo(point);

    newPolygonPath->setPath(polygonPath);
}

void MainWindow::changeInteractionMode(QAction *action)
{
    currentMode = static_cast<InteractionMode>(action->data().value<int>());
    actions->setEnabled(false);
    cancelAction->setEnabled(true);
    if (currentMode == AddNewPolygon)
        stopAction->setEnabled(true);
}

void MainWindow::stopActionTriggered()
{
    if (currentMode == AddNewPolygon)
        ; // Do stuff

    clearInteractionMode();
}

void MainWindow::cancelActionTriggered()
{
    if (currentMode == AddNewPolygon)
        newPolygonPath->setPath(QPainterPath());

    clearInteractionMode();
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
    case AddNewPolygon:
        addPointToNewPolygon(point);
        newPolygon->setVisible(true);
    case NoInteraction:
    default:
        break;
    }
}
