#include <QImage>
#include <QList>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <QtDebug>

#include "FieldView.h"

FieldView::FieldView(QWidget *parent)
    : QWidget(parent),
      columns(10),
      rows(18)
{
    const int w = 320;
    const int h = 576;
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
            int anID = 0;
            if (anID >= 0) {
                QRectF stoneRect(QPointF(x * stoneSize.width(),
                                         y * stoneSize.height()),
                                 stoneSize);
                painter.drawImage(stoneRect, stoneImages[anID]);
            }
        }
    }
}
