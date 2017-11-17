#include "hexboard.hpp"
#include <qmath.h>
#include <QDebug>

HexBoard::HexBoard()
{

}

HexBoard::~HexBoard()
{
    for(auto &hex: mHexes)
        delete hex;
}

void HexBoard::placeHexes()
{
    createHexColumn(60, 100, true, 7);
    createHexColumn(950, 100, false, 7);
    qDebug() << "mHexes.size() = " << mHexes.size();
    for(const auto &hex: mHexes)
    {
        qDebug() << hex->pos();
    }
}

void HexBoard::createHexColumn(const QPointF &pos, bool isRed, bool isPainted, size_t number)
{
    auto hexStep = 2 * Hex::RADIUS * qSin(2 * M_PI / Hex::NUM_SIDES) + 2;
    for(size_t i {0}; i < number; ++i)
    {
        Hex *hex = new Hex(pos.x(), pos.y() + hexStep * i, isRed);
        mHexes.append(hex);
        emit hexWasPlaced(hex);
    }
}

void HexBoard::createHexColumn(double x, double y, bool isRed, size_t number)
{
    createHexColumn(QPointF(x, y), isRed, number);
}
