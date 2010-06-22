#ifndef MYPLOT_H
#define MYPLOT_H

#include <QWidget>
#include <QPainter>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_math.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_text.h>

//const static int SECONDS = 101;

class Background: public QwtPlotItem
{
public:
    Background(int _x1, int _x2, int _d): x1(_x1), x2(_x2), d(_d)
    {
        setZ(0.0);
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRect &rect) const
    {
        QColor c(0, 130, 255);
        QRect r = rect;

        for ( int i = x1; i > x2; i -= d )
        {
            r.setBottom(yMap.transform(i - d));
            r.setTop(yMap.transform(i));
            painter->fillRect(r, c);

            c = c.light(106);
            //c = c.lighter(110);
        }
    }

    void changeSize(int _x1, int _x2, int _d)
    {
        x1 = _x1;
        x2 = _x2;
        d = _d;

        //replot();
    }

 private:
     int x1;
     int x2;
     int d;

};

class MyCurve: public QwtPlotCurve
{
public:
    MyCurve(const QString &title):
        QwtPlotCurve(title)
    {
        setRenderHint(QwtPlotItem::RenderAntialiased);
    }

    void setColor(const QColor &color)
    {
        QColor c = color;
        c.setAlpha(150);

        setPen(c);
    }

};

class BasePlot: public QwtPlot
{
public:
    BasePlot(bool two = false, QWidget *parent = 0)
        : QwtPlot(parent)
    {
        setAutoReplot(false);

        plotLayout()->setAlignCanvasToScales(true);

        // background
        //Background *bg = new Background(x1, x2, d);
        //bg->attach(this);
        //QWidget::setWindowOpacity(qreal(0.5));

        // grid
        QwtPlotGrid *grid = new QwtPlotGrid;
        //grid->enableXMin(true);
        grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
        //grid->setMinPen(QPen(QColor(160, 82, 45), 0, Qt::DotLine));
        grid->attach(this);

        // axes
        //enableAxis(QwtPlot::yRight);
        //setAxisTitle(QwtPlot::xBottom, "Normalized Frequency");
        //setAxisTitle(QwtPlot::yLeft, "Amplitude [dB]");
        //setAxisTitle(QwtPlot::yRight, "Phase [deg]");

        setAxisMaxMajor(QwtPlot::xBottom, 20);
        setAxisMaxMinor(QwtPlot::xBottom, 5);

        //setAxisMaxMajor(QwtPlot::yLeft, 20);
        //setAxisMaxMinor(QwtPlot::yLeft, 5);

        if(two)
        {
            enableAxis(QwtPlot::yRight);
            //setAxisMaxMajor(QwtPlot::yLeft, 20);
            //setAxisMaxMinor(QwtPlot::yLeft, 5);
        }

    }


};

class Plot1: public BasePlot
{
public:
    Plot1(QWidget *parent);

    /*
    const QwtPlotCurve *getCurve() const
    { return curve; }
    */
    ~Plot1();

protected:
    void timerEvent(QTimerEvent *e);

private:
        MyCurve *curve;
        QwtSymbol *sb;

        const int count;
        double *data;
        double *time;

};

class Plot2: public BasePlot
{
public:
    Plot2(QWidget *parent);

    ~Plot2();

    void changScale(int x, int d);

protected:
    void timerEvent(QTimerEvent *e);

private:
        MyCurve *curve1;
        MyCurve *curve2;

        Background *bg;

        const int count;
        double *data1;
        double *data2;
        double *time;

};


class Plot3: public BasePlot
{
public:
    Plot3(QWidget *parent);

    /*
    const QwtPlotCurve *getCurve() const
    { return curve; }
    */
    ~Plot3();

protected:
    void timerEvent(QTimerEvent *e);

private:
        MyCurve *curve;

        const int count;
        double *data;
        double *time;

};

#endif // MYPLOT_H
