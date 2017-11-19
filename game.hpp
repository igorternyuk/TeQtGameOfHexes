#pragma once

#include <QGraphicsView>
#include <QList>
#include "hex.hpp"
#include "button.hpp"


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
private:
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
    bool mIsGameOver { false };
    bool mIsRedPlayersTurn { true };
    bool mIsHexSelected { false };
    Hex *mSelectedHex { nullptr };
    QList<Hex*> mRedPlayerHexes;
    QList<Hex*> mBluePlayerHexes;
    QGraphicsTextItem *mWhoseTurnLabel;
    size_t mNumOfPlacedHexes;
    Button *mButton;
private:
    void createHexesForPlayers();
    GameResult determineWinner();
    Hex *findClosestHex(Hex* selectedHex);
    double distanceBetweenHexes(Hex* hex1, Hex *hex2) const;
private slots:
    void pickHexUp(Hex *hex);
    void placeHex();
    void createGUI();
    inline void switchTurn() { mIsRedPlayersTurn = !mIsRedPlayersTurn; }
    void updateTurnLabel();
    void centralizeWhoseTurnLabel();



};
