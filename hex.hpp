#pragma once
#include <QGraphicsPolygonItem>

class Hex : public QGraphicsPolygonItem
{
public:
    enum { NUM_SIDES = 6, RADIUS = 50 };
    explicit Hex(bool isRed = true, bool isPainted = false,
                 QGraphicsItem *parent = nullptr);
    explicit Hex(double x, double y, bool isRed = true,
                 bool isPainted = false, QGraphicsItem *parent = nullptr);
    explicit Hex(const QPointF &pos, bool isRed = true,
                 bool isPainted = false, QGraphicsItem *parent = nullptr);
    inline int getAttackOf(int side = 1) { return mAttackValues[side]; }
    inline void setAttackValue(int side, int value) { mAttackValues[side] = value; }
private:
    int mAttackValues[NUM_SIDES];
};
