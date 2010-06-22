/*
 * =====================================================================================
 *
 *       Filename:  mylabel.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年05月31日 19时38分39秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>

#include "mylabel.h"

MyLabel::MyLabel(const QString &_l1, const QString &_l2, const QString &_l3, QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    l0 = new QLabel(this);
    l1 = new QLabel(_l1, this);
    l2 = new QLabel(_l2, this);
    l3 = new QLabel(_l3, this);
    l4 = new QLineEdit(this);

    //QFontMetrics fm(font());

    l0->setFixedWidth(24);
    l1->setFixedWidth(140);
    l2->setFixedWidth(30);
    l3->setFixedWidth(20);
    l3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    l4->setMaximumWidth(50);
    l4->setReadOnly(true);
    l4->setAlignment(Qt::AlignCenter);
    //l4->setContentsMargins(3,1,1,3);
    l4->installEventFilter(this);


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(2,2,2,2);
    layout->addWidget(l0);
    layout->addWidget(l1);
    layout->addStretch(10);
    layout->addWidget(l2);
    layout->addSpacing(20);
    layout->addWidget(l3);
    layout->addSpacing(5);
    layout->addWidget(l4);
}

bool MyLabel::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        emit clicked(num);
        return true;
    }
    else
        return QObject::eventFilter(obj, event);
}

void MyLabel::changStatus(bool on)
{
    QString tmp = on ? QString(":/sources/button_ok1.png") : QString("");

    l0->setPixmap(tmp);
}

