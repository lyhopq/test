#include "modifydialog.h"
#include "ui_modifydialog.h"

#include <QMessageBox>
#include <QDebug>

#include "tobacco.h"
#include "pwddialog.h"

extern sysVal *sysval;
//extern MyDB *dm;
extern pwdDialog *pwdDia;

modifyDialog::modifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modifyDialog)
{
    ui->setupUi(this);
    ui->name->setText(sysval->userMap[sysval->userID].at(0));

    ui->cipher1->installEventFilter(this);
    ui->cipher2->installEventFilter(this);
    ui->cipher3->installEventFilter(this);
}

modifyDialog::~modifyDialog()
{
    delete ui;
}

void modifyDialog::changeEvent(QEvent *e)
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

void modifyDialog::setUser(QString &user)
{
    ui->name->insert(user);
}

void modifyDialog::on_pb_ok_clicked()
{
    if(ui->cipher1->text() != sysval->userMap[sysval->userID].at(1))
        QMessageBox::critical(0, tr("原密码输入错误！"),
            tr("你的输入为：") + ui->cipher1->text());
    else if(ui->cipher2->text() != ui->cipher3->text())
        QMessageBox::critical(0, tr("输入不配陪！"),
            tr("确认密码与更改密码不一致"));
    else
    {
        //sysval->pwd = ui->cipher2->text();
        //sysval->userMap[sysval->userID].at(1) = ui->cipher2->text();

        QString tmp = QString("update user set pwd='%1' where name='%2';")
            .arg(ui->cipher2->text())
            .arg(sysval->userMap[sysval->userID].at(0));
        //qDebug() << tmp;
        if(!sysval->dm->query.exec(tmp))
        {
            QMessageBox::critical(0, tr("写入数据库有误！"),
                tr("请检查数据库"), QMessageBox::Cancel);
        }
        else
        {
            QStringList tmp;
            tmp << sysval->userMap[sysval->userID].at(0)
                << ui->cipher2->text()
                << sysval->userMap[sysval->userID].at(2)
                << sysval->userMap[sysval->userID].at(3);

            sysval->userHand(2, sysval->userID, tmp);

            QMessageBox::information(0, tr("确定"), tr("密码修改成功！"),
                QMessageBox::Ok);
        }

       accept();
    }
}

bool modifyDialog::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        if(pwdDia->exec() == QDialog::Accepted)
        {
            (static_cast<QLineEdit *>(obj))->insert(pwdDia->getPwd());
            return true;
        }
        else
            return false;

    }
    else
        return QObject::eventFilter(obj, e);
}

