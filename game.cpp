#include "game.hpp"
#include "hexboard.hpp"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <qmath.h>
#include <QMessageBox>
#ifdef DEBUG
#include <QDebug>
#endif

Game::Game(QWidget *parent):
    QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setWindowTitle(mTitle);
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
}

Game::~Game()
{
    delete mHexBoard;
}

void Game::run()
{
    this->show();
}

void Game::prepareNewGame()
{
    //mIsGameOver = false;
    for(size_t i {0}; i < NUM_OF_HEXES_FOR_PLAYER; ++i)
    {
        mRedPlayerHexes.at(i)->generateRandomSideAttackValues();
        mBluePlayerHexes.at(i)->generateRandomSideAttackValues();
    }

    for(auto &hex: mHexBoard->getHexes())
    {
        hex->setType(Hex::Type::Neutral);
        hex->resetSideAttackValues();
        hex->setSideAttackLabelsVisible(false);
    }
    mNumOfPlacedHexes = 0;
    mIsRedPlayersTurn = true;
    mIsHexSelected = false;
    mSelectedHex = nullptr;
    this->updateTurnLabel();
}

void Game::mouseReleaseEvent(QMouseEvent *event)
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
                    mHexBoard->removeHex(releasedHex);
                    if(mNumOfPlacedHexes > 0)
                        mSelectedHex->conquerNeighbours();
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
                        gameOver();
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

void Game::keyPressEvent(QKeyEvent *event)
{
    auto key = event->key();
    if(key == Qt::Key_Space)
    {
        prepareNewGame();
    }
    else if(key == Qt::Key_Escape)
    {
        auto answer = QMessageBox::question(this, "Confirm exit",
                                            "Do you really want to exit?",
                                            QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::Yes)
            this->close();
    }
    else
    {
        QGraphicsView::keyPressEvent(event);
    }
}

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

void Game::createGUI()
{
    QGraphicsRectItem *leftRect = new QGraphicsRectItem();
    leftRect->setRect(QRectF(0,0,150,768));
    leftRect->setPos(0,0);
    leftRect->setBrush(QBrush(QColor(127,51,86)));
    QGraphicsTextItem *redPlayerLabel =
            new QGraphicsTextItem("First\nplayer", leftRect);
    redPlayerLabel->setFont(QFont("times", 24));
    redPlayerLabel->setDefaultTextColor(Qt::yellow);
    mScene->addItem(leftRect);
    redPlayerLabel->setPos((leftRect->boundingRect().width() -
                            redPlayerLabel->boundingRect().width()) / 2, 10);

    QGraphicsRectItem *rightRect = new QGraphicsRectItem();
    rightRect->setRect(QRectF(0,0,150,768));
    rightRect->setPos(WINDOW_WIDTH - 150,0);
    rightRect->setBrush(QBrush(QColor(107,220,120)));
    QGraphicsTextItem *bluePlayerLabel =
            new QGraphicsTextItem("Second\nplayer", rightRect);
    bluePlayerLabel->setFont(QFont("times", 24));
    bluePlayerLabel->setDefaultTextColor(Qt::darkGreen);
    mScene->addItem(rightRect);
    bluePlayerLabel->setPos((rightRect->boundingRect().width() -
                             bluePlayerLabel->boundingRect().width()) / 2, 10);

    mWhoseTurnLabel = new QGraphicsTextItem("Turn");
    mWhoseTurnLabel->setFont(QFont("times", 24));
    mWhoseTurnLabel->setDefaultTextColor(Qt::blue);
    auto dx = (WINDOW_WIDTH - mWhoseTurnLabel->boundingRect().width()) / 2;
    mWhoseTurnLabel->setPos(dx, 20);
    mScene->addItem(mWhoseTurnLabel);
}

void Game::updateTurnLabel()
{
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
    //if(!mIsGameOver) return GameResult::NOT_DEFINED;
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

void Game::gameOver()
{
    //mIsGameOver = true;
    auto res = determineWinner();
    QString msg;
    if(res == GameResult::REDS_WON)
        msg = QString("Reds won!!!");
    else if(res == GameResult::BLUES_WON)
        msg = QString("Blues won!!!");
    else
        msg = QString("Draw!");
    auto answer = QMessageBox::question(this, msg, "Play again?",
                                        QMessageBox::Yes | QMessageBox::No);
    if(answer == QMessageBox::Yes)
        this->prepareNewGame();
    else if(answer == QMessageBox::No)
        this->close();
}
