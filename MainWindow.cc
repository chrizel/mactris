#include <cmath>

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtDebug>

#include "FieldView.h"
#include "GameView.h"
#include "MainWindow.h"

static Block blocks[BLOCK_COUNT][BLOCK_STATES] = {
    // Block 1: XX
    //          XX
    { { {0,0,0,0}, {0,0,1,1}, {0,0,1,1}, {0,0,0,0} },
      { {0,0,0,0}, {0,0,1,1}, {0,0,1,1}, {0,0,0,0} },
      { {0,0,0,0}, {0,0,1,1}, {0,0,1,1}, {0,0,0,0} },
      { {0,0,0,0}, {0,0,1,1}, {0,0,1,1}, {0,0,0,0} } },

    // Block 2: XXX
    //           X
    { { {0,0,2,0}, {0,0,2,2}, {0,0,2,0}, {0,0,0,0} },
      { {0,0,2,0}, {0,2,2,2}, {0,0,0,0}, {0,0,0,0} },
      { {0,0,2,0}, {0,2,2,0}, {0,0,2,0}, {0,0,0,0} },
      { {0,0,0,0}, {0,2,2,2}, {0,0,2,0}, {0,0,0,0} } },

    // Block 3: XXX
    //            X
    { { {0,0,3,0}, {0,0,3,0}, {0,0,3,3}, {0,0,0,0} },
      { {0,0,0,3}, {0,3,3,3}, {0,0,0,0}, {0,0,0,0} },
      { {0,3,3,0}, {0,0,3,0}, {0,0,3,0}, {0,0,0,0} },
      { {0,0,0,0}, {0,3,3,3}, {0,3,0,0}, {0,0,0,0} } },

    // Block 4: XXX
    //          X  
    { { {0,0,4,4}, {0,0,4,0}, {0,0,4,0}, {0,0,0,0} },
      { {0,4,0,0}, {0,4,4,4}, {0,0,0,0}, {0,0,0,0} },
      { {0,0,4,0}, {0,0,4,0}, {0,4,4,0}, {0,0,0,0} },
      { {0,0,0,0}, {0,4,4,4}, {0,0,0,4}, {0,0,0,0} } },

    // Block 5:  XX
    //          XX
    { { {0,0,0,5}, {0,0,5,5}, {0,0,5,0}, {0,0,0,0} },
      { {0,5,5,0}, {0,0,5,5}, {0,0,0,0}, {0,0,0,0} },
      { {0,0,0,5}, {0,0,5,5}, {0,0,5,0}, {0,0,0,0} },
      { {0,5,5,0}, {0,0,5,5}, {0,0,0,0}, {0,0,0,0} } },

    // Block 6: XX
    //           XX
    { { {0,0,6,0}, {0,0,6,6}, {0,0,0,6}, {0,0,0,0} },
      { {0,0,6,6}, {0,6,6,0}, {0,0,0,0}, {0,0,0,0} },
      { {0,0,6,0}, {0,0,6,6}, {0,0,0,6}, {0,0,0,0} },
      { {0,0,6,6}, {0,6,6,0}, {0,0,0,0}, {0,0,0,0} } },

    // Block 7: XXXX
    { { {0,0,7,0}, {0,0,7,0}, {0,0,7,0}, {0,0,7,0} },
      { {0,0,0,0}, {7,7,7,7}, {0,0,0,0}, {0,0,0,0} },
      { {0,0,7,0}, {0,0,7,0}, {0,0,7,0}, {0,0,7,0} },
      { {0,0,0,0}, {7,7,7,7}, {0,0,0,0}, {0,0,0,0} } }
};


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      curBlockData(0)
{
    scoreLabel = new QLabel;
    scoreLabel->setMinimumWidth(90);
    levelLabel = new QLabel;
    levelLabel->setMinimumWidth(90);

    fieldView = new GameView(this, FIELD_COLUMNS, FIELD_ROWS, 32);
    nextStoneView = new FieldView(this, 4, 2, 16);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 5, 0, 5);
    vbox->addWidget(new QLabel(tr("Next Stone:")));
    vbox->addWidget(nextStoneView);
    vbox->addSpacing(20);
    vbox->addWidget(scoreLabel);
    vbox->addWidget(levelLabel);
    vbox->addStretch();

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->addWidget(fieldView, 0, Qt::AlignTop);
    hbox->addLayout(vbox);
    setLayout(hbox);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    newGame();
}

MainWindow::~MainWindow()
{
}

int MainWindow::idOfStone(FieldView *aFieldView, const int x, const int y)
{
    int stoneData;

    if (aFieldView == fieldView) {
        stoneData = field[x][y];
        if ((stoneData == 0) && (curBlockData != 0)) {
            // Empty stone data on field... read stone data from block?
            if ( ( (x >= curX) && (x < curX + BLOCK_MAX_WIDTH) )
                && (y >= curY) && (y < curY + BLOCK_MAX_HEIGHT) ) {
                stoneData = (*curBlockData)[x - curX][y - curY];
            }
        }
    } else {
        stoneData = (blocks[nextBlock][0])[x][y+2];
    }

    return stoneData;
}

void MainWindow::newGame()
{
    memset(field, 0, FIELD_COLUMNS * FIELD_ROWS * sizeof(int));

    qsrand(time(NULL));
    qsrand(qrand());

    nextBlock = qrand() % BLOCK_COUNT;
    takeNextBlock();

    score = 0;
    level = 1;
    fast = false;
    pause = false;
    gameOver = false;

    updateTimer();
    updateViews();
}

void MainWindow::pauseGame()
{
    pause = !pause;
    updateTimer();
    updateViews();
}

void MainWindow::takeNextBlock()
{
    curX = 3;
    curY = -1;
    curState = 0;
    curBlock = nextBlock;
    nextBlock = qrand() % BLOCK_COUNT;
    updateBlockData();
}

void MainWindow::updateBlockData()
{
    curBlockData = &blocks[curBlock][curState];
}

void MainWindow::updateTimer()
{
    timer.stop();

    if (!pause && !gameOver) {
        int msec = fast
                 ? 80
                 : 1000 - 1000 * log2((float)level)
                   / log2((float)MAX_LEVEL + (MAX_LEVEL * 0.3333));
        timer.start(msec);
    }
}

void MainWindow::updateViews()
{
    fieldView->update();
    nextStoneView->update();

    scoreLabel->setText(tr("Score: <b>%1</b>").arg(score));
    levelLabel->setText(tr("Level: <b>%1</b>").arg(level));
}

void MainWindow::timerTick()
{
    curY++;
    if (checkCollision()) {
        curY--;
        copyCurBlockToField();
        takeNextBlock();

        // game over?
        if (checkCollision()) {
            gameOver = true;
            updateViews();
            timer.stop();

            int result = QMessageBox::question(this,
                tr("Game Over"), tr("Do you like to play again?"),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (result == QMessageBox::Yes) {
                newGame();
            } else {
                close();
            }
        }
    }

    // Calculate score...
    int deletedRows = checkRows();
    for (int i = 1; i <= deletedRows; i++)
        score += i * 30 * level;

    // Need level update?
    if (needLevelUpdate()) {
        level++;
        updateTimer();
    }

    updateViews();
}

bool MainWindow::checkCollision()
{
    for (int y = 0; y < BLOCK_MAX_HEIGHT; y++) {
        for (int x = 0; x < BLOCK_MAX_WIDTH; x++) {
            if ((*curBlockData)[x][y] != 0) {
                // Field border collision?
                if ( ((curX + x) < 0) || ((curX + x) >= FIELD_COLUMNS) || ((curY + y) >= FIELD_ROWS) )
                    return true;

                // Field stone collision?
                if ( ((curY + y) >= 0) && field[curX + x][curY + y] != 0 )
                    return true;
            }
        }
    }

    return false;
}

void MainWindow::copyCurBlockToField()
{
    for (int y = 0; y < BLOCK_MAX_HEIGHT; y++) {
        for (int x = 0; x < BLOCK_MAX_WIDTH; x++) {
            if ((*curBlockData)[x][y] != 0) {
                field[curX + x][curY + y] = (*curBlockData)[x][y];
            }
        }
    }
}

int MainWindow::checkRows()
{
    int deletedRows = 0;
    for (int y = 0; y < FIELD_ROWS; y++) {
        int stoneCount = 0;
        for (int x = 0; x < FIELD_COLUMNS; x++) {
            if (field[x][y] != 0) {
                stoneCount++;
            }
        }

        if (stoneCount == FIELD_COLUMNS) {
            deleteRow(y);
            deletedRows++;
        }
    }

    return deletedRows;
}

void MainWindow::deleteRow(const int aRow)
{
    for (int y = aRow; y > 1; y--) {
        for (int x = 0; x < FIELD_COLUMNS; x++) {
            field[x][y] = field[x][y-1];
        }
    }
}

bool MainWindow::needLevelUpdate() 
{
    if (level >= MAX_LEVEL)
        return false;

    int scoreLimit = 0;
    for (int i = 1; i <= level; i++)
        scoreLimit += 1000 * i;

    return score >= scoreLimit;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_P) {
        pauseGame();
        return;
    } else if (pause || gameOver) {
        QWidget::keyPressEvent(event);
        return;
    }

    Block *oldBlockData = curBlockData;
    int oldX = curX,
        oldY = curY;
    switch (event->key()) {
    case Qt::Key_Up:
        curState = curState >= BLOCK_STATES - 1 ? 0 : curState + 1;
        updateBlockData();
        break;
    case Qt::Key_Left:
        curX--;
        break;
    case Qt::Key_Right:
        curX++;
        break;
    case Qt::Key_Down:
        if (!fast) {
            fast = true;
            updateTimer();
            timerTick();
        }
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    if (checkCollision()) {
        curBlockData = oldBlockData;
        curX = oldX;
        curY = oldY;
    }

    updateViews();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!pause && 
        !gameOver && 
        !event->isAutoRepeat() && 
        (event->key() == Qt::Key_Down)) 
    {
        fast = false;
        updateTimer();
        return;
    }

    QWidget::keyPressEvent(event);
}
