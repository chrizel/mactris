#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QWidget>

#include "FieldView.h"
#include "GameView.h"
#include "MainWindow.h"

GameView::GameView(MainWindow *mainWindow, int columns, int rows, 
                   int block_size, QWidget *parent)
    : FieldView(mainWindow, columns, rows, block_size, parent)
{
    font1 = QFont("Helvetica", 24, QFont::Bold);
    font2 = QFont("Helvetica", 16);
}

GameView::~GameView()
{
}

void GameView::paintEvent(QPaintEvent *event)
{
    FieldView::paintEvent(event);

    if (mainWindow->isPause() || mainWindow->isGameOver()) {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 0, 153));

        if (mainWindow->isPause()) {
            drawCenteredString(painter, tr("Game paused"), font1, 0);
            drawCenteredString(painter, tr("Press P to continue"), font2, 30);
        } else if (mainWindow->isGameOver()) {
            drawCenteredString(painter, tr("Game Over"), font1, 0);
        }
    }
}

void GameView::drawCenteredString(QPainter &painter, const QString &string,
                                  const QFont &font, int yoffset)
{
    const int shadowSize = 4;
    painter.setFont(font);
    QFontMetrics fm(painter.fontMetrics());
    QSize size(fm.size(0, string));
    QPoint point(rect().width() / 2 - size.width() / 2 + shadowSize,
                 rect().height() / 2 - size.height() / 2 + shadowSize + yoffset);
    for (int i = shadowSize; i >= 0; i--) {
        painter.setPen(i == 0 
                       ? Qt::white 
                       : QColor(0, 0, 0, (shadowSize - i) * 50));
        painter.drawText(point, string);
        point -= QPoint(1, 1);
    }
}
