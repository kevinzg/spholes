#include "PointItem.h"

#include <QBrush>
#include <QPen>

const qreal PointItem::defaultRadius = 5.0;

PointItem::PointItem(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    this->setRect(QRectF(-defaultRadius/2, -defaultRadius/2, defaultRadius, defaultRadius));
    this->setBrush(QBrush(Qt::SolidPattern));
    this->setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void PointItem::setColor(QColor color)
{
    this->setBrush(QBrush(color, Qt::SolidPattern));
    this->setPen(QPen(color));
}
