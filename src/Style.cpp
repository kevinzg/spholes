#include "Style.h"

const QColor Style::startPointColor = QColor(224, 43, 42);
const QColor Style::destinationPointColor = QColor(255, 133, 16);

const QPen Style::obstaclePen = QPen(QColor(63, 111, 160));
const QBrush Style::obstacleBrush = QBrush(QColor(79, 139, 201, 128));

const QColor Style::newPolygonStartPointColor = QColor(63, 111, 160);
const QPen Style::newPolygonPen = QPen(QColor(63, 111, 160));

const QPen Style::shortestPathPen = QPen(QBrush(QColor(41, 42, 45)), 1.2, Qt::SolidLine);
const QPen Style::visibilityGraphPen = QPen(QBrush(QColor(89, 92, 102)), 0.5, Qt::DashLine);
