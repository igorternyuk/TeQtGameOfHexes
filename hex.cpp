#include "hex.hpp"
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <qmath.h>
#include <QGraphicsSceneMouseEvent>
#include "random.h"
#include <QFont>
#ifdef DEBUG
#include <QDebug>
#endif

Hex::Hex(QGraphicsItem *parent):
    Hex(QPointF(), Type::Neutral, parent)
{}

Hex::Hex(double x, double y, Type type,
         QGraphicsItem *parent):
    Hex(QPointF(x, y), type, parent)
{}

Hex::Hex(const QPointF &pos, Type type,
         QGraphicsItem *parent):
    QGraphicsPolygonItem(parent), mInitialPos(pos)
{
    this->setPos(pos);
    mType = type;
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

    switch (mType) {
    case Type::Red:
        this->setBrush(Qt::red);
        break;
    case Type::Blue:
        this->setBrush(Qt::blue);
        break;
    default:
        this->setBrush(Qt::lightGray);
        break;
    }

    //Collision detectors

    for(auto i = 0.5f; i < NUM_SIDES; ++i)
    {
        auto angle = 360 / NUM_SIDES * i;
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(
                    QLineF::fromPolar(RADIUS + 7, angle), this);
        lineItem->setVisible(false);
        mCollisionDetectors.append(lineItem);
    }

    //Side attack value labels

    for(auto i = 0.5f; i < NUM_SIDES; ++i)
    {
        auto angle = 2 * M_PI / NUM_SIDES * i;
        auto r = 0.7f * RADIUS;
        QPointF pos(r * qCos(angle) - 0.21 * r,
                    r * qSin(angle) - 0.39 * r);
        QGraphicsTextItem *textItem = new QGraphicsTextItem("0", this);
        textItem->setPos(pos);
        textItem->setFont(QFont("times", 10));
        textItem->setDefaultTextColor(Qt::yellow);
        mSideAttackLabels.append(textItem);
    }

    if(mType == Type::Neutral)
        setSideAttackLabelsVisible(false);

}

void Hex::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

QPointF Hex::getInitialPos() const
{
    return mInitialPos;
}

Hex::Type Hex::getType() const
{
    return mType;
}

void Hex::setType(const Type &type)
{
    mType = type;
}

QList<Hex *> Hex::detectNeighbours()
{
    auto scene = this->scene();
}

void Hex::generateRandomSideAttackValues()
{
    Random random;
    for(size_t i{0}; i < NUM_SIDES; ++i)
    {
        auto attackVal = random.nextInt(1, 6);
        this->setAttackValue(i, attackVal);
        mSideAttackLabels.at(i)->setPlainText(QString::number(attackVal));
    }
}

void Hex::setSideAttackLabelsVisible(bool isVisible)
{
    for(const auto &textItem: mSideAttackLabels)
        textItem->setVisible(isVisible);
}
