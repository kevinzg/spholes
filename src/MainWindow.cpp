#include "MainWindow.h"
#include "View.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene(this);

    view = new View(tr("Euclidean Shortest Path"), this);
    view->view()->setScene(scene);
    this->setCentralWidget(view);
}

MainWindow::~MainWindow()
{

}
