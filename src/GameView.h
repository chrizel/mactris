#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "FieldView.h"

class MainWindow;
class QPaintEvent;
class QWidget;

class GameView : public FieldView
{
    Q_OBJECT

private:
    QFont font1;
    QFont font2;

    void drawCenteredString(QPainter &painter, const QString &string, 
                            const QFont &font, int yoffset);

public:
    GameView(MainWindow *mainWindow, int columns, int rows, int block_size, QWidget *parent = 0);
    virtual ~GameView();

    void paintEvent(QPaintEvent *event);
};

#endif
