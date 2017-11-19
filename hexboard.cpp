#include "hexboard.hpp"
#include <qmath.h>
#include <QDebug>

HexBoard::HexBoard(QObject *parent):
    QObject(parent)
{

}

void HexBoard::placeHexes()
{
    createGrid(205, 100, Hex::Type::Neutral, 9, 7, 2, 1);
}

void HexBoard::removeHex(Hex *hex)
{
    mHexes.removeAll(hex);
    delete hex;
}

void HexBoard::createHexColumn(const QPointF &pos, Hex::Type type,
                               size_t number, double margin_y)
{
    auto hexStep = 2 * Hex::RADIUS * qSin(2 * M_PI / Hex::NUM_SIDES) + 2;
    for(size_t i {0}; i < number; ++i)
    {
        Hex *hex = new Hex(pos.x(), pos.y() + (hexStep + margin_y) * i, type);
        mHexes.append(hex);
        emit hexWasPlaced(hex);
    }
}

void HexBoard::createHexColumn(double x, double y, Hex::Type type,
                               size_t number)
{
    createHexColumn(QPointF(x, y), type, number);
}

void HexBoard::createGrid(double x, double y, Hex::Type type, size_t num_x,
                          size_t num_y, double margin_x, double margin_y)
{
    createGrid(QPointF(x,y), type, num_x, num_y, margin_x, margin_y);
}

void HexBoard::createGrid(const QPointF &pos, Hex::Type type, size_t num_x,
                          size_t num_y, double margin_x, double margin_y)
{
    auto hexOffsetX = Hex::RADIUS + Hex::RADIUS * qSin(M_PI / Hex::NUM_SIDES) + margin_x;
    auto hexOffsetY = Hex::RADIUS * qSin(2 * M_PI / Hex::NUM_SIDES) + margin_y;
    for(size_t nx {0}; nx < num_x; ++nx)
    {
        QPointF curr_pos(pos.x() + nx * hexOffsetX, nx % 2 == 0 ?
                             pos.y() : pos.y() + hexOffsetY);
        createHexColumn(curr_pos, type, num_y, margin_y);
    }
}
