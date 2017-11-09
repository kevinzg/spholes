#include "MainWindow.h"
#include "View.h"
#include "Style.h"
#include "spholes/ShortestPath.h"

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
    startPoint->setColor(Style::startPointColor);
    startPoint->setVisible(false);

    destinationPoint = new PointItem;
    destinationPoint->setColor(Style::destinationPointColor);
    destinationPoint->setVisible(false);

    obstacleGroup = new QGraphicsItemGroup;

    newPolygon = new QGraphicsItemGroup;
    newPolygon->setVisible(false);

    newPolygonStartPoint = new PointItem;
    newPolygonStartPoint->setColor(Style::newPolygonStartPointColor);

    newPolygonPath = new QGraphicsPathItem;
    newPolygonPath->setPen(Style::newPolygonPen);

    newPolygon->addToGroup(newPolygonStartPoint);
    newPolygon->addToGroup(newPolygonPath);

    shortestPath = new QGraphicsPathItem;
    shortestPath->setPen(Style::shortestPathPen);

    scene->addItem(startPoint);
    scene->addItem(destinationPoint);
    scene->addItem(obstacleGroup);
    scene->addItem(newPolygon);
    scene->addItem(shortestPath);
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

    solveAction = new QAction(tr("Solve"), this);
    solveAction->setEnabled(false);

    connect(solveAction, &QAction::triggered,
            this, &MainWindow::solveActionTriggered);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = new QToolBar(this);
    this->addToolBar(toolBar);

    toolBar->addActions(actions->actions());
    toolBar->addAction(stopAction);
    toolBar->addAction(cancelAction);
    toolBar->addSeparator();
    toolBar->addAction(solveAction);
}

void MainWindow::clearInteractionMode()
{
    actions->checkedAction()->setChecked(false);
    actions->setEnabled(true);

    stopAction->setEnabled(false);
    cancelAction->setEnabled(false);
    solveAction->setEnabled(startPoint->isVisible() && destinationPoint->isVisible());

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

void MainWindow::drawShortestPath(const spholes::Path &path)
{
    if (path.size() < 2) return;

    QPainterPath painterPath;

    painterPath.moveTo(path[0].x(), path[0].y());

    for (size_t i = 1; i < path.size(); ++i)
        painterPath.lineTo(path[i].x(), path[i].y());

    shortestPath->setPath(painterPath);
}

void MainWindow::changeInteractionMode(QAction *action)
{
    currentMode = static_cast<InteractionMode>(action->data().value<int>());

    actions->setEnabled(false);
    solveAction->setEnabled(false);
    cancelAction->setEnabled(true);

    if (currentMode == AddNewPolygon)
        stopAction->setEnabled(true);
}

void MainWindow::stopActionTriggered()
{
    if (currentMode == AddNewPolygon)
    {
        QGraphicsPolygonItem *obstacle = new QGraphicsPolygonItem(newPolygonPath->path().toFillPolygon());
        obstacle->setPen(Style::obstaclePen);
        obstacle->setBrush(Style::obstacleBrush);
        obstacleGroup->addToGroup(obstacle);

        newPolygonPath->setPath(QPainterPath());
    }

    clearInteractionMode();
}

void MainWindow::cancelActionTriggered()
{
    if (currentMode == AddNewPolygon)
        newPolygonPath->setPath(QPainterPath());

    clearInteractionMode();
}

void MainWindow::solveActionTriggered()
{
    spholes::Point start(startPoint->pos().x(), startPoint->pos().y());
    spholes::Point destination(destinationPoint->pos().x(), destinationPoint->pos().y());
    std::vector<spholes::Polygon> obstacles;

    foreach (QGraphicsItem *item, obstacleGroup->childItems())
    {
        QGraphicsPolygonItem *graphicsPolygon = dynamic_cast<QGraphicsPolygonItem *>(item);

        if (!graphicsPolygon)
            continue;

        obstacles.push_back(spholes::Polygon());

        foreach (const QPointF &point, graphicsPolygon->polygon())
            obstacles.rbegin()->push_back(spholes::Point(point.x(), point.y()));
    }

    spholes::Path solution = spholes::ShortestPath::find(start, destination, obstacles);
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
