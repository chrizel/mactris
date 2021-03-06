#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QWidget>

const int MAX_LEVEL = 10;

const int BLOCK_COUNT = 7;
const int BLOCK_STATES = 4;
const int BLOCK_MAX_WIDTH = 4;
const int BLOCK_MAX_HEIGHT = 4;

const int FIELD_ROWS = 18;
const int FIELD_COLUMNS = 10;

typedef int Block[BLOCK_MAX_WIDTH][BLOCK_MAX_HEIGHT];

class FieldView;
class QKeyEvent;
class QLabel;

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    QLabel *scoreLabel;
    QLabel *levelLabel;
    FieldView *fieldView;
    FieldView *nextStoneView;

    int field[FIELD_COLUMNS][FIELD_ROWS];
    Block *curBlockData;

    int score;
    int level;
    int curX;
    int curY;
    int curState;
    int nextBlock;
    int curBlock;

    QTimer timer;

    bool fast;
    bool pause;
    bool gameOver;

    void newGame();
    void pauseGame();
    void takeNextBlock();
    void updateBlockData();

    void updateTimer();
    void updateViews();

    bool checkCollision();
    void copyCurBlockToField();
    int checkRows();
    void deleteRow(const int aRow);

    bool needLevelUpdate();
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    int idOfStone(FieldView *aFieldView, const int x, const int y);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    inline bool isPause() { return pause; };
    inline bool isGameOver() { return gameOver; };

protected slots:
    void timerTick();
};

#endif
