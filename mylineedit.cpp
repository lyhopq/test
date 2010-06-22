#include <QMouseEvent>

#include "mylineedit.h"

MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

void MyLineEdit::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        emit getFocus();
}
