#ifndef POINTITEM_H
#define POINTITEM_H

#include <QGraphicsEllipseItem>

class PointItem : public QGraphicsEllipseItem
{
    static const qreal defaultRadius;

public:
    PointItem(QGraphicsItem *parent = 0);

    void setColor(QColor color);
};

#endif // POINTITEM_H
