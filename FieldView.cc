#include <QImage>
#include <QList>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QtDebug>

#include "FieldView.h"
#include "MainWindow.h"

FieldView::FieldView(MainWindow *mainWindow, int columns, int rows, 
                     int block_size, QWidget *parent)
    : QWidget(parent),
      mainWindow(mainWindow),
      columns(columns),
      rows(rows)
{
    const int w = block_size * columns;
    const int h = block_size * rows;
    setMinimumSize(w, h);
    setMaximumSize(w, h);

    stoneImages << QImage("gfx/red_stone.png")
                << QImage("gfx/green_stone.png")
                << QImage("gfx/cyan_stone.png")
                << QImage("gfx/blue_stone.png")
                << QImage("gfx/yellow_stone.png")
                << QImage("gfx/orange_stone.png")
                << QImage("gfx/magenta_stone.png");
}

FieldView::~FieldView()
{
}

void FieldView::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    // Draw background
    painter.fillRect(rect(), QBrush(QImage("gfx/bg.png")));

    // Calculate stone size on basis of bounds
    QSizeF stoneSize(rect().width() / columns,
                     rect().height() / rows);

    // Draw stones
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            int anID = mainWindow->idOfStone(this, x, y);
            if (anID > 0) {
                QRectF stoneRect(QPointF(x * stoneSize.width(),
                                         y * stoneSize.height()),
                                 stoneSize);
                painter.drawImage(stoneRect, stoneImages[anID - 1]);
            }
        }
    }
}
