/*
 * =====================================================================================
 *
 *       Filename:  myplot.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010年06月07日 15时14分22秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

//#include <QtGui>

#include <QPen>
#include <QBrush>
#include "myplot.h"

Plot1::Plot1(QWidget *parent)
    : BasePlot(true, parent), count(129)
{
    // curve
    curve = new MyCurve(tr("紧头监视"));
    curve->setColor(Qt::green);

    QPen pen(QColor(Qt::black));
    QBrush brush(Qt::yellow);

    sb = new QwtSymbol(QwtSymbol::Ellipse, brush, pen, QSize(6,6));
    curve->setSymbol(*sb);

    curve->attach(this);

    // background
    Background *bg = new Background(15, -15, 2);
    bg->attach(this);
    //QWidget::setWindowOpacity(qreal(0.5));



    time = new double[count];
    data = new double[count];
    for(int i = 0; i< count; i++)
    {
        time[count-1-i] = i - 64;
        data[i] = 0;
    }

    setAxisScale(QwtPlot::xBottom, time[count - 1], time[0]);
    setAxisScale(QwtPlot::yLeft, -15, 15);
    setAxisScale(QwtPlot::yRight, -15, 15);
    curve->setRawData(time, data, count);

    startTimer(1000); // 1 second
}

Plot1::~Plot1()
{
    delete[] time;
    delete[] data;
}

void Plot1::timerEvent(QTimerEvent *)
{
    static bool f = true;
    if(f)
    {
        sb->setBrush(QColor(Qt::red));
        curve->setColor(QColor(Qt::red));
    }
    else
    {
        sb->setBrush(QColor(Qt::green));
        curve->setColor(QColor(Qt::green));
    }

    f = !f;

    curve->setSymbol(*sb);

    for(int i = count -1; i > 0; i--)
    {
        data[i] = data[i-1];
    }

    static int magic = rand()%100 +1;

    magic = (magic-10)%14;

    data[0] = magic;

    //curve->setRawData(time, data, SECONDS);


    replot();
}

Plot2::Plot2(QWidget *parent)
    : BasePlot(true, parent), count(101)
{
    // curve
    curve1 = new MyCurve(tr("内排平均重量"));
    curve1->setColor(Qt::red);
    QPen pen(QColor(Qt::black));
    QBrush brush(Qt::red);
    QwtSymbol sb(QwtSymbol::Diamond, brush, pen, QSize(6,6));
    curve1->setSymbol(sb);
    curve1->attach(this);

    curve2 = new MyCurve(tr("外排平均重量"));
    curve2->setColor(Qt::green);
    QBrush brush2(Qt::green);

    QwtSymbol sb2(QwtSymbol::Rect, brush2, pen, QSize(6,4));
    curve2->setSymbol(sb2);
    curve2->attach(this);


    // background
    bg = new Background(15, -15, 2);
    bg->attach(this);
    //QWidget::setWindowOpacity(qreal(0.5));



    time = new double[count];
    data1 = new double[count];
    data2 = new double[count];
    for(int i = 0; i< count; i++)
    {
        time[count-1-i] = i;
        data1[i] = 0;
        data2[i] = 0;
    }

    setAxisScale(QwtPlot::xBottom, time[count - 1], time[0]);
    //setAxisScale(QwtPlot::yLeft, -15, 15);
    //setAxisScale(QwtPlot::yRight, -15, 15);
    curve1->setRawData(time, data1, count);
    curve2->setRawData(time, data2, count);

    changScale(20, 4);

    startTimer(1000); // 1 second
}

Plot2::~Plot2()
{
    delete[] time;
    delete[] data1;
    delete[] data2;
}

void Plot2::changScale(int x, int d)
{
    bg->changeSize(x , -x, d);

    setAxisScale(QwtPlot::yLeft, -x, x);
    setAxisScale(QwtPlot::yRight, -x, x);

    replot();
}

void Plot2::timerEvent(QTimerEvent *)
{
    /*
    static bool f = true;
    if(f)
    {
        sb->setBrush(QColor(Qt::red));
        curve->setColor(QColor(Qt::red));
    }
    else
    {
        sb->setBrush(QColor(Qt::green));
        curve->setColor(QColor(Qt::green));
    }

    f = !f;

    curve->setSymbol(*sb);
    */

    for(int i = count -1; i > 0; i--)
    {
        data1[i] = data1[i-1];
        data2[i] = data2[i-1];
    }

    static int magic = rand()%100 +1;

    magic = (magic-10)%14;

    data1[0] = magic;
    data2[0] = -magic;

    //curve->setRawData(time, data, SECONDS);


    replot();
}



Plot3::Plot3(QWidget *parent)
    : BasePlot(parent), count(101)
{
    // curve
    curve = new MyCurve(tr("实时重量"));
    curve->setColor(Qt::red);

    QPen pen(QColor(Qt::black));
    QBrush brush(Qt::yellow);

    QwtSymbol sb(QwtSymbol::Rect, brush, pen, QSize(6,4));
    curve->setSymbol(sb);

    curve->attach(this);

    // background
    Background *bg = new Background(400, -400, 50);
    bg->attach(this);
    //QWidget::setWindowOpacity(qreal(0.5));



    time = new double[count];
    data = new double[count];
    for(int i = 0; i< count; i++)
    {
        time[count-1-i] = i;
        data[i] = 0;
    }

    setAxisScale(QwtPlot::xBottom, time[count - 1], time[0]);
    setAxisScale(QwtPlot::yLeft, -400, 400);
    curve->setRawData(time, data, count);

    startTimer(1000); // 1 second
}

Plot3::~Plot3()
{
    delete[] time;
    delete[] data;
}

void Plot3::timerEvent(QTimerEvent *)
{
    for(int i = count -1; i > 0; i--)
    {
        data[i] = data[i-1];
    }

    static int magic = rand()%100 +1;

    magic = (magic+10)%400;

    data[0] = magic;

    //curve->setRawData(time, data, SECONDS);


    replot();
}

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget vBox;

    vBox.setWindowTitle("Example");

    QLinearGradient linearGrad(QPointF(20, 100), QPointF(20, 200));
    linearGrad.setColorAt(0, QColor(0, 130, 198));
    //linearGrad.setColorAt(0.5, QColor(140, 199, 231));
    linearGrad.setColorAt(1, QColor(180, 220, 240));

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(linearGrad));
    vBox.setPalette(palette);

    MyPlot *plot = new MyPlot(&vBox);

    //plot->setMargin(5);

    QString info("Just a test!");

    QLabel *label = new QLabel(info, &vBox);

    QVBoxLayout *layout = new QVBoxLayout(&vBox);
    layout->addWidget(plot);
    layout->addWidget(label);

    vBox.resize(600, 400);
    vBox.show();

    return a.exec();
}
*/
