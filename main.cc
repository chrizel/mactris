#include <QApplication>

#include "FieldView.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FieldView fieldView;
    fieldView.show();
    return app.exec();
}
