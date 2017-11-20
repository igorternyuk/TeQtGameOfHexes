#pragma once

#include <QGraphicsView>
#include <QList>
#include "hex.hpp"


class QGraphicsScene;
class HexBoard;
class QGraphicsTextItem;
class QGraphicsRectItem;

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    void run();
    void prepareNewGame();
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    const QString mTitle {"TeQtGameOfHexes"};
    enum { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 768, NUM_OF_HEXES_FOR_PLAYER = 5 };
    enum class GameResult
    {
        REDS_WON,
        BLUES_WON,
        DRAW,
        NOT_DEFINED
    };
    QGraphicsScene *mScene;
    HexBoard *mHexBoard;
   // bool mIsGameOver { false };
    bool mIsRedPlayersTurn { true };
    bool mIsHexSelected { false };
    Hex *mSelectedHex { nullptr };
    QList<Hex*> mRedPlayerHexes;
    QList<Hex*> mBluePlayerHexes;
    QGraphicsTextItem *mWhoseTurnLabel;
    int mNumOfPlacedHexes {0};
private:
    void createHexesForPlayers();
    GameResult determineWinner();
    void gameOver();
private slots:
    void createGUI();
    inline void switchTurn() { mIsRedPlayersTurn = !mIsRedPlayersTurn; }
    void updateTurnLabel();
    void centralizeWhoseTurnLabel();



};
