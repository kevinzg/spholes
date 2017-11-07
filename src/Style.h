#ifndef STYLE_H
#define STYLE_H

#include <QtGui/QColor>
#include <QtGui/QBrush>
#include <QtGui/QPen>

class Style
{
public:
    static const QColor startPointColor;
    static const QColor destinationPointColor;

    static const QPen obstaclePen;
    static const QBrush obstacleBrush;

    static const QColor newPolygonStartPointColor;
    static const QPen newPolygonPen;
};

#endif // STYLE_H
