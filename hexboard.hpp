#pragma once

#include <QObject>
#include <QList>
#include "hex.hpp"

class HexBoard: public QObject
{
    Q_OBJECT
public:
    explicit HexBoard(QObject *parent = nullptr);
    //~HexBoard();
    void placeHexes();
    void removeHex(Hex *hex);
    inline QList<Hex*>& getHexes() { return mHexes; }
signals:
    void hexWasPlaced(Hex *hex);
private:
    QList<Hex*> mHexes;
    void createHexColumn(const QPointF &pos, Hex::Type type, size_t number,
                         double margin_y = 0);
    void createHexColumn(double x, double y, Hex::Type type, size_t number);
    void createGrid(double x, double y, Hex::Type type, size_t num_x,
                    size_t num_y, double margin_x, double margin_y);
    void createGrid(const QPointF &pos, Hex::Type type, size_t num_x,
                    size_t num_y, double margin_x, double margin_y);
};
