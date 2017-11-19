#pragma once

#include <QList>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QLineF>

class Hex : public QGraphicsPolygonItem
{

public:
    enum class Type { Red, Blue, Neutral };
    enum { NUM_SIDES = 6, RADIUS = 50 };

    //Constructors

    explicit Hex(QGraphicsItem *parent = nullptr);
    explicit Hex(double x, double y, Type type,
                 QGraphicsItem *parent = nullptr);
    explicit Hex(const QPointF &pos, Type type,
                 QGraphicsItem *parent = nullptr);
    inline int getAttackOf(int side = 1)
    {
        return mAttackValues[side];
    }
    inline void resetPos() { this->setPos(mInitialPos); }
    void generateRandomSideAttackValues();
    void setSideAttackLabelsVisible(bool isVisible = true);
    Type getType() const;
    void setType(const Type &type);
    QPointF getInitialPos() const;

signals:

protected:

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF mInitialPos;
    Type mType { Type::Neutral };
    int mAttackValues[NUM_SIDES] {0};
    QList<QGraphicsTextItem*> mSideAttackLabels;
    QList<QGraphicsLineItem*> mCollisionDetectors;
    QList<Hex*> detectNeighbours();
    inline void setAttackValue(int side, int value)
    {
        mAttackValues[side] = value;
    }

};
