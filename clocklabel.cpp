#include <QDateTime>

#include "clocklabel.h"


ClockLabel::ClockLabel(QWidget *parent)
    : QLabel(parent)
{
    setMargin(10);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //timerEvent(0);
    startTimer(1000);
}

void ClockLabel::timerEvent(QTimerEvent */*event*/)
{
    QDateTime datetime = QDateTime::currentDateTime();

    int y = datetime.date().year();
    int m = datetime.date().month();
    int d = datetime.date().day();
    QString w = tranWeek(datetime.date().dayOfWeek());
    QString strTime = datetime.time().toString("HH:mm:ss");

    setText(tr("今天： %1年%2月%3日 星期%4 %5")
        .arg(y).arg(m).arg(d)
        .arg(w).arg(strTime));
}

QString ClockLabel::tranWeek(int d)
{
    switch(d)
    {
        case 1:
            return tr("一");
            break;
        case 2:
            return tr("二");
            break;
        case 3:
            return tr("三");
            break;
        case 4:
            return tr("四");
            break;
        case 5:
            return tr("五");
            break;
        case 6:
            return tr("六");
            break;
        case 7:
            return tr("日");
            break;
        default:
            return "";
    }
}
