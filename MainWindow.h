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

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    int field[FIELD_COLUMNS][FIELD_ROWS];
    FieldView *fieldView;
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

protected slots:
    void timerTick();
};

#endif
