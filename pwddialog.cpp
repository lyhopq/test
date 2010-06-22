#include "pwddialog.h"
#include "ui_pwddialog.h"

#include <QDebug>

#include "tobacco.h"

extern sysVal *sysval;

pwdDialog::pwdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pwdDialog)
{
    ui->setupUi(this);

    QList<QPushButton *> keyList = ui->keyFrame->findChildren<QPushButton *>();

    //qDebug() << "size:" << keyList.size();

    for (int i = 0; i < keyList.size(); i++)
    {
        connect(keyList[i], SIGNAL(clicked()), this, SLOT(getKey()));
    }


}

pwdDialog::~pwdDialog()
{
    delete ui;
}

void pwdDialog::changeEvent(QEvent *e)
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

void pwdDialog::getKey()
{
    QString c = (static_cast<const QPushButton*>(this->sender()))->text();

    if (c.toUtf8() == "\342\206\220")
        ui->pwdLineEdit->backspace();
    else if (c == "Enter" || c == "Space" || c == "+" || c == "-" || c == ".")
        ;
    else
        ui->pwdLineEdit->insert(c);
}

void pwdDialog::on_okButton_clicked()
{
    //sysval->pwd = ui->pwdLineEdit->text();

    //this->res = sysval->pwd;
    this->res = ui->pwdLineEdit->text();

    ui->pwdLineEdit->clear();
    accept();
}

void pwdDialog::on_cancelButton_clicked()
{
    ui->pwdLineEdit->clear();

    reject();
}
