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
    updateColor();

    //Collision detectors
    auto angle_step = 360 / NUM_SIDES;
    auto lenthOfLines = Hex::RADIUS + 7;
    for(auto angle = angle_step / 2; angle < 360; angle += angle_step)
    {
        auto line = QLineF::fromPolar(lenthOfLines, angle);
        QGraphicsLineItem *lineItem = new QGraphicsLineItem(
                    line, this);
        lineItem->setVisible(false);
        mCollisionDetectors.append(lineItem);
    }

    //Side attack value labels
    auto distToLabel = 0.7f * Hex::RADIUS;
    size_t counter {0u};
    for(auto angle = angle_step / 2; angle < 360; angle += angle_step)
    {
        auto line = QLineF::fromPolar(distToLabel, angle);
        QGraphicsTextItem *textItem =
                new QGraphicsTextItem(QString::number(counter++), this);
        QPointF pos(line.p2().x() - 0.21 * distToLabel,
                    line.p2().y() - 0.39 * distToLabel);
        textItem->setPos(pos);
        textItem->setFont(QFont("times", 10));
        textItem->setDefaultTextColor(Qt::yellow);
        mSideAttackLabels.append(textItem);
    }

    if(mType == Type::Neutral)
        setSideAttackLabelsVisible(false);

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
    updateColor();
}

QMap<int, Hex*> Hex::detectNeighbours()
{
    QMap<int, Hex*> neighbours;
    for(auto i = 0; i < mCollisionDetectors.size(); ++i)
    {
        auto colliders = mCollisionDetectors[i]->collidingItems();
        for(const auto &item: colliders)
        {
            Hex* hex = dynamic_cast<Hex*>(item);
            if(hex && hex->pos() != this->pos())
            {
                neighbours.insert(i, hex);
            }
        }
    }
    return neighbours;
}

void Hex::updateColor()
{
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
}

unsigned int Hex::conquerNeighbours()
{
    unsigned int numOfConqueredNeighbours { 0u };
    qDebug() << "conquerNeighbours()";
    qDebug() << "My attack values";
    for(int i = 0; i < 6; ++i)
    {
        qDebug() << QString("getAttackOf(%1) = %2").arg(i).arg(getAttackOf(i));
    }
    auto neighbours = this->detectNeighbours();
    for(auto it = neighbours.begin(); it != neighbours.end(); ++it)
    {
        auto side = it.key();
        auto neighbour = it.value();
        if(neighbour->getType() != Hex::Type::Neutral &&
           neighbour->getType() != this->getType())
        {

            auto thisAttackValue = getAttackOf(side);
            qDebug() << "val1 = " << thisAttackValue;
            auto neighbourAttackValue = (side < 3) ?
                                        neighbours[side]->getAttackOf(side + 3) :
                                        neighbours[side]->getAttackOf(side - 3);
            qDebug() << "val2 = " << neighbourAttackValue;
            if(thisAttackValue > neighbourAttackValue)
            {
                neighbours[side]->setType(this->getType());
                ++numOfConqueredNeighbours;
            }
        }
    }
    return numOfConqueredNeighbours;
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

void Hex::resetSideAttackValues()
{
    for(size_t i{0}; i < NUM_SIDES; ++i)
    {
        this->setAttackValue(i, 0);
        mSideAttackLabels.at(i)->setPlainText(QString::number(0));
    }
}

void Hex::setSideAttackLabelsVisible(bool isVisible)
{
    for(const auto &textItem: mSideAttackLabels)
        textItem->setVisible(isVisible);
}
