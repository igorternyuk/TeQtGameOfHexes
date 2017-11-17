#include "game.hpp"
#include "hex.hpp"
#include "hexboard.hpp"
#include <QGraphicsScene>

Game::Game(QWidget *parent):
    QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWindowTitle("TeQtGameOfHexes");
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mScene = new QGraphicsScene(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, this);
    this->setScene(mScene);
    mHexBoard = new HexBoard();
    connect(mHexBoard, &HexBoard::hexWasPlaced, [this](const auto &hex){
        mScene->addItem(hex);
    });
    mHexBoard->placeHexes();
}

Game::~Game()
{
    delete mHexBoard;
}

void Game::run()
{
    this->show();
}
