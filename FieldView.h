#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QImage>
#include <QList>
#include <QWidget>

class QPaintEvent;
class MainWindow;

class FieldView : public QWidget
{
    Q_OBJECT

private:
    MainWindow *mainWindow;
    int columns;
    int rows;
    QList<QImage> stoneImages;

public:
    FieldView(MainWindow *mainWindow, int columns, int rows, int block_size);
    virtual ~FieldView();

    void paintEvent(QPaintEvent *event);
};

#endif
