#include <QWidget>

#include "MainWindow.h"
#include "FieldView.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    fieldView = new FieldView(this);
}

MainWindow::~MainWindow()
{
}

int MainWindow::idOfStone(FieldView *aFieldView, const int x, const int y)
{
    return (qrand() % 8) - 1;
}
