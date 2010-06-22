#include <QtGui/QApplication>
#include <QFile>
#include <QTextCodec>
//#include <QWindowsStyle>
//#include <QGtkStyle>
#include <QDebug>
#include "index.h"

#include "tobacco.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    //a.setStyle(new QGtkStyle);


    QFile qss(":/sources/tobacco.css");
    if(qss.open(QIODevice::ReadOnly))
    {
        a.setStyleSheet(qss.readAll());
        qss.close();
    }

    Index w;
    qDebug() << QApplication::arguments();
    if(QApplication::arguments().contains("-em"))
    {
        w.showFullScreen();
    }
    else
        w.setFixedSize(800,600);

    w.show();
    return a.exec();
}
