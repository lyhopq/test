#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>


class MyLineEdit: public QLineEdit
{
    Q_OBJECT

    public:
        MyLineEdit(QWidget *parent = 0);

    protected:
        void mousePressEvent(QMouseEvent *);

    signals:
        void getFocus();
};
#endif // MYLINEEDIT_H
