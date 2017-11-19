#include "game.hpp"
#include "hexboard.hpp"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <qmath.h>
#include <QMessageBox>
#include <QDebug>

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
    createGUI();
    mHexBoard->placeHexes();
    createHexesForPlayers();
    this->setMouseTracking(true);
    centralizeWhoseTurnLabel();
    //mButton = new Button(QPointF(10, 10), "Button");
    //mScene->addItem(mButton);

}

Game::~Game()
{
    delete mHexBoard;
    for(auto &hex: mRedPlayerHexes)
    {
        mScene->removeItem(hex);
        delete hex;
    }
    mRedPlayerHexes.clear();
    for(auto &hex: mBluePlayerHexes)
    {
        mScene->removeItem(hex);
        delete hex;
    }
    mBluePlayerHexes.clear();
}

void Game::run()
{
    this->show();
}

void Game::prepareNewGame()
{

}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if(!mIsGameOver)
    {
        QPointF scenePoint = mapToScene(event->pos());
        QGraphicsItem *releasedItem = mScene->itemAt(scenePoint, transform());
        Hex *releasedHex = dynamic_cast<Hex*>(releasedItem);
        if(releasedHex)
        {
            if(event->button() == Qt::MouseButton::LeftButton)
            {
                if(mIsHexSelected)
                {
                    for(auto &item: mScene->items(scenePoint))
                    {
                        Hex *hex = dynamic_cast<Hex*>(item);
                        if(hex && hex->getType() == Hex::Type::Neutral &&
                           mHexBoard->getHexes().contains(hex))
                        {
                            releasedHex = hex;
                            break;
                        }
                    }
                    if(mNumOfPlacedHexes < mHexBoard->getHexes().size())
                    {
                        auto newHexPos = mSelectedHex->getInitialPos();
                        auto targetPos = releasedHex->pos();
                        mSelectedHex->setPos(targetPos);
                        mHexBoard->getHexes().append(mSelectedHex);
                        mHexBoard->getHexes().removeAll(releasedHex);
                        delete releasedHex;
                        ++mNumOfPlacedHexes;
                        Hex *newHex = new Hex(newHexPos, mIsRedPlayersTurn ?
                                              Hex::Type::Red : Hex::Type::Blue);
                        newHex->generateRandomSideAttackValues();
                        newHex->setSideAttackLabelsVisible(true);
                        mScene->addItem(newHex);
                        if(mIsRedPlayersTurn)
                            mRedPlayerHexes.append(newHex);
                        else
                            mBluePlayerHexes.append(newHex);
                        if(mNumOfPlacedHexes >= mHexBoard->getHexes().size())
                            determineWinner();
                        else
                            switchTurn();
                        updateTurnLabel();
                        mIsHexSelected = false;
                        mSelectedHex = nullptr;

                    }
                }
                else
                {
                    bool isRedMoveValid { mIsRedPlayersTurn &&
                                          releasedHex->getType() == Hex::Type::Red &&
                                          mRedPlayerHexes.contains(releasedHex)};
                    bool isBlueMoveValid { !mIsRedPlayersTurn &&
                                           releasedHex->getType() == Hex::Type::Blue &&
                                           mBluePlayerHexes.contains(releasedHex)};
                    if(isRedMoveValid || isBlueMoveValid)
                    {
                        mSelectedHex = releasedHex;
                        mIsHexSelected = true;
                    }
                }
            }
            else if(event->button() == Qt::MouseButton::RightButton)
            {
                if(mIsHexSelected)
                {
                    mIsHexSelected = false;
                    mSelectedHex->resetPos();
                }
            }
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(mIsHexSelected)
    {
        QPointF scenePoint = mapToScene(event->pos());
        mSelectedHex->setPos(scenePoint);
    }
    QGraphicsView::mouseMoveEvent(event);
}

/*Hex* Game::createHex(const QPointF &pos, Hex::Type type)
{
    Hex *hex = new Hex(pos, type);
}*/

void Game::createHexesForPlayers()
{
    auto hexStepY = 2 * Hex::RADIUS * qSin(2 * M_PI / Hex::NUM_SIDES) + 10;
    QPointF topLeftCornerOfRedHexesStack(75, 200);
    QPointF topLeftCornerOfBlueHexesStack(WINDOW_WIDTH - 75, 200);
    for(size_t i {0}; i < NUM_OF_HEXES_FOR_PLAYER; ++i)
    {
        Hex *hexR = new Hex(topLeftCornerOfRedHexesStack.x(),
                            topLeftCornerOfRedHexesStack.y() + i * hexStepY,
                            Hex::Type::Red);
        hexR->generateRandomSideAttackValues();
        hexR->setSideAttackLabelsVisible(true);
        mRedPlayerHexes.append(hexR);
        mScene->addItem(hexR);
        Hex *hexB = new Hex(topLeftCornerOfBlueHexesStack.x(),
                            topLeftCornerOfBlueHexesStack.y() + i * hexStepY,
                            Hex::Type::Blue);
        hexB->generateRandomSideAttackValues();
        hexB->setSideAttackLabelsVisible(true);
        mBluePlayerHexes.append(hexB);
        mScene->addItem(hexB);
    }
}

void Game::pickHexUp(Hex *hex)
{

}

void Game::placeHex()
{

}

void Game::createGUI()
{
    QGraphicsRectItem *leftRect = new QGraphicsRectItem();
    leftRect->setRect(QRectF(0,0,150,768));
    leftRect->setPos(0,0);
    leftRect->setBrush(QBrush(QColor(127,51,86)));
    QGraphicsTextItem *redPlayerLabel = new QGraphicsTextItem("First\nplayer", leftRect);
    redPlayerLabel->setFont(QFont("times", 24));
    redPlayerLabel->setDefaultTextColor(Qt::yellow);
    mScene->addItem(leftRect);
    redPlayerLabel->setPos(5, 10);

    QGraphicsRectItem *rightRect = new QGraphicsRectItem();
    rightRect->setRect(QRectF(0,0,150,768));
    rightRect->setPos(WINDOW_WIDTH - 150,0);
    rightRect->setBrush(QBrush(QColor(107,220,120)));
    QGraphicsTextItem *bluePlayerLabel = new QGraphicsTextItem("Second\nplayer", rightRect);
    bluePlayerLabel->setFont(QFont("times", 24));
    bluePlayerLabel->setDefaultTextColor(Qt::darkGreen);
    mScene->addItem(rightRect);
    bluePlayerLabel->setPos(5, 10);

    mWhoseTurnLabel = new QGraphicsTextItem("Turn");
    mWhoseTurnLabel->setFont(QFont("times", 24));
    mWhoseTurnLabel->setDefaultTextColor(Qt::blue);
    auto dx = (WINDOW_WIDTH - mWhoseTurnLabel->boundingRect().width()) / 2;
    mWhoseTurnLabel->setPos(dx, 10);
    mScene->addItem(mWhoseTurnLabel);
}

void Game::updateTurnLabel()
{
    if(mIsGameOver)
    {
        //mWhoseTurnLabel->setT
    }
    if(mIsRedPlayersTurn)
    {
        mWhoseTurnLabel->setDefaultTextColor(Qt::red);
        mWhoseTurnLabel->setPlainText("Reds to move");
    }
    else
    {
        mWhoseTurnLabel->setDefaultTextColor(Qt::blue);
        mWhoseTurnLabel->setPlainText("Blues to move");
    }
    centralizeWhoseTurnLabel();
}

void Game::centralizeWhoseTurnLabel()
{
    auto dx = (WINDOW_WIDTH - mWhoseTurnLabel->boundingRect().width()) / 2;
    mWhoseTurnLabel->setPos(dx, 5);
}

Game::GameResult Game::determineWinner()
{
    if(!mIsGameOver) return GameResult::NOT_DEFINED;
    auto hexes = mHexBoard->getHexes();
    size_t counterReds { 0u };
    size_t counterBlues { 0u };
    for(const auto &hex: hexes)
    {
        if(hex->getType() == Hex::Type::Red)
                ++counterReds;
        else if(hex->getType() == Hex::Type::Blue)
                ++counterBlues;
    }
    if(counterReds > counterBlues) return GameResult::REDS_WON;
    if(counterReds < counterBlues) return GameResult::BLUES_WON;
    if(counterReds == counterBlues) return GameResult::DRAW;
    return GameResult::NOT_DEFINED;
}
