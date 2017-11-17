#include "hex.hpp"
#include <QVector>
#include <QPointF>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <qmath.h>

Hex::Hex(bool isRed, bool isPainted, QGraphicsItem *parent):
    Hex(QPointF(), isRed, isPainted, parent)
{}

Hex::Hex(double x, double y, bool isRed, bool isPainted,
         QGraphicsItem *parent):
    Hex(QPointF(x, y), isRed, isPainted, parent)
{}

Hex::Hex(const QPointF &pos, bool isRed, bool isPainted,
         QGraphicsItem *parent)
{
    QVector<QPointF> points;
    for(int i {0}; i < NUM_SIDES; ++i)
    {
        auto angle = i * 2 * M_PI / NUM_SIDES;
        points << QPointF(RADIUS * qCos(angle), RADIUS * qSin(angle));
    }
    QPolygonF polygon(points);
    this->setPolygon(polygon);
    QPen pen;
    pen.setWidth(1);
    pen.setBrush(Qt::black);
    this->setPen(pen);
    if(isPainted)
        this->setBrush(isRed ? Qt::red : Qt::blue);
    this->setPos(pos);
}
