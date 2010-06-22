#include <QHeaderView>
#include <QDebug>

#include "mysqltableview.h"

MySqlModel::MySqlModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    setAttr(2);
}

void MySqlModel::setAttr(int n, const QColor &_c3, const QColor &_c1, const QColor &_c2)
{
    count = n;

    c1 = _c1;
    c2 = _c2;
    c3 = _c3;
}

QVariant MySqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);

    /*
    if(value.isValid() && role = Qt::DisplayRole)
    {

    }
    */
    if(role == Qt::TextColorRole && index.column() == 1)
        return qVariantFromValue(c1);
    if(role == Qt::TextColorRole && index.column() == 0 && index.row() == 8)
        return qVariantFromValue(c1);
    if(role == Qt::TextColorRole && index.column() == 2)
        return qVariantFromValue(c2);

    for(int i = 0; i < count; i++)
    {
        if(role == Qt::BackgroundRole && index.column() == 2+i)
            return qVariantFromValue(c3);
    }

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    return value;
}


MyTableView::MyTableView(QAbstractItemModel *model, QWidget *parent)
    : QTableView(parent)
{
    //setFixedWidth(400);
    QHeaderView* headerView = verticalHeader();
    headerView->setHidden(true);
    headerView->setResizeMode(QHeaderView::Stretch);

    headerView = horizontalHeader();
    headerView->setResizeMode(QHeaderView::Stretch);

    setEnabled(false);
    setFocusPolicy(Qt::NoFocus);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setModel(model);

    //qDebug() << this->frameGeometry() << this->frameSize() << this->width() << this->height()
    //    << this->maximumSize() << this->size() << this->sizeHint();

    /*
    int c = this->model()->columnCount();
    int w = 680/c;
    for(int i = 0; i < c; i++)
        setColumnWidth(i, w);

    int r = this->model()->rowCount();
    int h = 288/r;
    for(int i = 0; i < r; i++)
        setRowHeight(i, h);
    */
}

MyTableView1::MyTableView1(QSqlQueryModel *model, QString table, QWidget *parent)
{
    model->setQuery(QString("select * from %1;").arg(table));

    model->setHeaderData(0, Qt::Horizontal, tr("序号"));
    model->setHeaderData(1, Qt::Horizontal, tr("故障代码"));
    model->setHeaderData(2, Qt::Horizontal, tr("时间（分钟）"));
    model->setHeaderData(3, Qt::Horizontal, tr("次数"));
    model->setHeaderData(4, Qt::Horizontal, tr("停机原因"));

    QHeaderView *hv = horizontalHeader();
    //hv->setResizeMode(QHeaderView::Stretch);
    hv->setResizeMode(QHeaderView::Fixed);
    hv->setStretchLastSection(true);

    hv = verticalHeader();
    hv->setHidden(true);

    setModel(model);

    setColumnWidth(0,60);
    setColumnWidth(1,100);
    setColumnWidth(2,100);
    setColumnWidth(3,60);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setCurrentIndex(model->index(0,0));

    //setFocusPolicy(Qt::NoFocus);
}
