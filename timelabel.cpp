/*
 * =====================================================================================
 *
 *       Filename:  timelabel.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2010年06月01日 16时32分28秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>

#include "timelabel.h"

TimeLabel::TimeLabel(QWidget *parent)
    : QFrame(parent)
{
    //setMinimumSize(100, 50);
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Raised);
    //setLineWidth(2);

    label = new QLabel(this);

    label->setAlignment(Qt::AlignCenter);
    //label->setMargin(5);
    label->installEventFilter(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 3, 5, 3);
    layout->addWidget(label);
}

/*
void TimeLabel::setTime(const QString &time)
{
    label->setText(time);
}
*/

void TimeLabel::setColor(bool on, const QColor &bc)
{
    if(on)
    {
        QPalette palette;
        palette.setColor(label->backgroundRole(), bc);
        label->setPalette(palette);
        label->setContentsMargins(10, 2, 10, 2);
    }
    else
    {
        label->setFrameShadow(QFrame::Sunken);
        label->setFrameShape(QFrame::Panel);
    }
    label->setAutoFillBackground(true);
}

bool TimeLabel::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        emit tClicked(num);
        return true;
    }
    else
        return QObject::eventFilter(obj, e);
}
