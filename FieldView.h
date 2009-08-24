#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QImage>
#include <QList>
#include <QWidget>

class QPaintEvent;

class FieldView : public QWidget
{
    Q_OBJECT

private:
    int columns;
    int rows;
    QList<QImage> stoneImages;

public:
    FieldView(QWidget *parent = 0);
    virtual ~FieldView();

    void paintEvent(QPaintEvent *event);
};

#endif
