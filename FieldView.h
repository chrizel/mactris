#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include <QWidget>

class FieldView : public QWidget
{
    Q_OBJECT

public:
    FieldView(QWidget *parent = 0);
    virtual ~FieldView();
};

#endif
