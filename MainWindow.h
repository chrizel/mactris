#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class FieldView;

class MainWindow : public QWidget
{
    Q_OBJECT

private:
    FieldView *fieldView;

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    int idOfStone(FieldView *aFieldView, const int x, const int y);
};

#endif
