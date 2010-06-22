#ifndef MYSQLTABLEVIEW_H
#define MYSQLTABLEVIEW_H

#include <QTableView>
#include <QSqlQueryModel>
#include <QColor>

class MySqlModel: public QSqlQueryModel
{
    Q_OBJECT

    public:
        MySqlModel(QObject *parent = 0);

        QVariant data(const QModelIndex &item, int role) const;

        void setAttr(int n,
                     const QColor &_c3 = QColor(0, 130, 132),
                     const QColor &_c1 = QColor(Qt::blue),
                     const QColor &_c2 = QColor(Qt::red));

    private:
        QColor c1;
        QColor c2;
        QColor c3;
        int count;
};


class MyTableView: public QTableView
{
    Q_OBJECT

    public:
        MyTableView(QAbstractItemModel *model, QWidget *parent = 0);

};

class MyTableView1: public QTableView
{
    Q_OBJECT

    public:
        MyTableView1(QSqlQueryModel *model, QString table, QWidget *parent = 0);
};

/*
class MyTableView2: public QTableView
{
    Q_OBJECT

    public:
        MyTableView2(QSqlQueryModel *model, QString table, QWidget *parent = 0);
};
*/


#endif // MYSQLTABLEVIEW_H
