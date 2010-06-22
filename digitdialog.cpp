#include "digitdialog.h"
#include "ui_digitdialog.h"

#include <QKeyEvent>
#include <QMessageBox>
//#include <QDebug>
//#include <QValidator>

digitDialog::digitDialog(const QString& _l1, const QString& _l2, /*int _type,*/ QWidget *parent)
    : QDialog(parent),/* type(_type),*/l1(_l1), l2(_l2),
    ui(new Ui::digitDialog)
{
    ui->setupUi(this);

    ui->label->setText(tr("最小: %1").arg(l1));
    ui->label2->setText(tr("最大: %1").arg(l2));

    ui->pb1->installEventFilter(this);
    ui->pb2->installEventFilter(this);
    ui->pb3->installEventFilter(this);
    ui->pb4->installEventFilter(this);
    ui->pb5->installEventFilter(this);
    ui->pb6->installEventFilter(this);
    ui->pb7->installEventFilter(this);
    ui->pb8->installEventFilter(this);
    ui->pb9->installEventFilter(this);
    ui->pb0->installEventFilter(this);
    //ui->pbm->installEventFilter(this);
    ui->pbj->installEventFilter(this);
    ui->pbb->installEventFilter(this);

    //QValidator *val = new QIntValidator(0, 9999, this);
    //ui->lineEdit->setValidator(val);

    reg = new QRegExp("([0-1][0-9]|2[0-3]):([0-5][0-9])-([0-1][0-9]|2[0-3]):([0-5][0-9])");
}

digitDialog::~digitDialog()
{
    delete ui;
}

void digitDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool digitDialog::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *key;
    //qDebug() << "i am here!" << event->type();

    if(event->type() == QEvent::MouseButtonPress)
    {
        if(obj == ui->pbb)
            key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        else
            key = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier,
                (static_cast<QPushButton*>(obj))->text());

        //qDebug() << "i am here!" << key->key();
        QCoreApplication::postEvent(QApplication::focusWidget(), key);
        return true;
    }
    return false;
}

void digitDialog::on_pb_cancel_clicked()
{
    ui->lineEdit->clear();

    reject();
}

void digitDialog::on_pb_ok_clicked()
{
    res = ui->lineEdit->text();
    ui->lineEdit->clear();

    accept();
}

void digitDialog::changeLabel(const QString &_l1, const QString &_l2)
{
    l1 = _l1;
    l2 = _l2;
    ui->label->setText(tr("最小: %1").arg(l1));
    ui->label2->setText(tr("最大: %1").arg(l2));
}

void digitDialog::setType(int _type)
{
    type = _type;
    switch(type)
    {
        case 0:
        {
            ui->pbm->setText(":");
            ui->pbm->setEnabled(true);
            ui->pbm->installEventFilter(this);
            break;
        }
        case 1:
        {
            ui->pbm->setText(".");
            ui->pbm->setEnabled(true);
            ui->pbm->installEventFilter(this);
            break;
        }
        case 2:
        {
            ui->pbm->setText(".");
            ui->pbm->setDisabled(true);
            ui->pbm->removeEventFilter(this);
            break;
        }
        default:
            break;
    }
}

bool digitDialog::isFitting()
{
    bool ok = false;
    if(type == 0)
    {
        //int count = res.count(reg);

        if(reg->exactMatch(res))
        {
            reg->indexIn(res);
            int t[4];
            QString tmp;
            for(int i = 0; i < 4; i++)
            {
                tmp = reg->cap(i+1);
                //qDebug() << tmp;
                if(tmp.startsWith("0"))
                    t[i] = tmp.right(1).toInt();
                else
                    t[i] = tmp.toInt();
            }
            if(t[0]*60+t[1] > t[2]*60+t[3])
                QMessageBox::critical(0, tr("错误"), tr("结束时间小于起始时间！"));
            else
                ok = true;
        }
        else
            QMessageBox::critical(0, tr("错误"), tr("输入时间格式错误！\n\n例： 00:30-12:40"));
    }
    else
    {
        double r = res.toDouble();
        double x1 = l1.toDouble();
        double x2 = l2.toDouble();
        if(r < x1)
            QMessageBox::critical(0, tr("错误"), tr("输入值不能小于 %1").arg(x1));
        else if(r > x2)
            QMessageBox::critical(0, tr("错误"), tr("输入值不能大于 %1").arg(x2));
        else
            ok = true;
    }

    return ok;
}

