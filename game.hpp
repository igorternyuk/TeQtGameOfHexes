#pragma once

#include <QGraphicsView>


class QGraphicsScene;
class HexBoard;

class Game: public QGraphicsView
{
public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    void run();
private:
    enum { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768 };
    QString whoseTurn;
    QGraphicsScene *mScene;
    HexBoard *mHexBoard;
};
