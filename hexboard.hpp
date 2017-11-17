#pragma once

#include <QObject>
#include <QList>
#include "hex.hpp"

class HexBoard: public QObject
{
    Q_OBJECT
public:
    explicit HexBoard();
    ~HexBoard();
    void placeHexes();
signals:
    void hexWasPlaced(Hex *hex);
private:
    QList<Hex*> mHexes;
    void createHexColumn(const QPointF &pos, bool isRed, bool isPainted, size_t number);
    void createHexColumn(double x, double y, bool isRed, size_t number);
};
