#include "userdialog.h"
#include "ui_userdialog.h"

#include "tobacco.h"
#include "pwddialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QVector>

extern pwdDialog *pwdDia;
//extern MyDB *dm;
extern sysVal *sysval;

userDialog::userDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userDialog)
{
    ui->setupUi(this);

    /**
    QPushButton *u11 = new QPushButton("lyh");
    u11->setFixedHeight(33);
    ui->uLayout->addWidget(u11);
    u11->hide();
    ***/


    this->updateUser();

    //qDebug() << "Button size:" << allButtons.size();

    connect(ui->perButton, SIGNAL(clicked()), this, SLOT(previous()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(next()));

}

userDialog::~userDialog()
{
    delete ui;
}

void userDialog::changeEvent(QEvent *e)
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

void userDialog::updateUser()
{
    c1 = 0;
    //if(sysval->dirty & 0x01)
    {
        QLayoutItem *child;
        while ((child = ui->uLayout->takeAt(0)) != 0)
        {
            //qDebug() << "i am here!";
            child->widget()->setParent(NULL);
            delete child;
        }
    //ui->uLayout->removeItem(uLayout1);

        /***
        test = dm->query.exec("select name,pwd from user where name='Administratorlyh';");
        dm->query.last();
        if(test)
            ui->uLayout->addWidget(new MyPushButton(
                tr("管理员"),
                tr("管理员"),
                dm->query.value(1).toString()));

        dm->query.exec("select name,pwd from user where name!='Administratorlyh';");
        while(dm->query.next())
        {
            i++;
            ui->uLayout->addWidget(new MyPushButton(
                dm->query.value(0).toString(),
                dm->query.value(0).toString(),
                dm->query.value(1).toString()));
        }
        ***/
        //QMap<QString, QString>::const_iterator i = sysval->userMap.constBegin();
        QMap<int, QStringList>::const_iterator i = sysval->userMap.constBegin();

        /***
        ui->uLayout->addWidget(new MyPushButton(
            tr("管理员"),tr("管理员")));
        ++i;
        ***/

        while (i != sysval->userMap.constEnd())
        {

            ui->uLayout->addWidget(new MyPushButton(i.value().at(0), i.key()));
            ++i;
        }

        if(sysval->userCount < 9)
        {
            for(int j = 0; j< 9 - sysval->userCount; j++)
            {
                ui->uLayout->addWidget(new QPushButton);
            }
        }



        allButtons = ui->uScrollarea->findChildren<MyPushButton *>();

        //qDebug() << sysval->userCount;

        for (int i = 0; i <= sysval->userCount; i++)
        {
            connect(allButtons[i], SIGNAL(clicked()), this, SLOT(getPwd()));

            if(i > 9)
                allButtons[i]->hide();

        }

        buttonEnable();


        //sysval->dirty &= 0x10;

    }
}
void userDialog::previous()
{
    allButtons[c1+9]->hide();
    allButtons[--c1]->show();
    buttonEnable();
}

void userDialog::next()
{
    allButtons[c1+10]->show();
    allButtons[c1++]->hide();
    buttonEnable();
}

void userDialog::buttonEnable()
{

    if (c1 == 0)
        ui->perButton->setEnabled(false);
    else
        ui->perButton->setEnabled(true);

    //if (sysval->userCount > 10)
    if(c1 + 9 < sysval->userCount)
        ui->nextButton->setEnabled(true);
    else
        ui->nextButton->setEnabled(false);
}

void userDialog::getPwd()
{
    if (pwdDia->exec() == QDialog::Accepted)
    {
        // 密码判断
        //int tmp = static_cast<MyPushButton*>(this->sender())->id;
        int tmp = static_cast<MyPushButton*>(this->sender())->id;
        //sysval->pwd  = pwdDia->getPwd();

        //qDebug() << tmp << sysval->pwd;

        //if((static_cast<MyPushButton*>(this->sender())->pwd != sysval->pwd))
        //qDebug() << tmp << sysval->userMap[tmp] << sysval->pwd;
        //if((sysval->userMap[tmp].at(1) != sysval->pwd))
        if((sysval->userMap[tmp].at(1) != pwdDia->getPwd()))
        {
            QMessageBox::critical(0,tr("输入密码不正确！"),
                tr("你输入的密码为：") + pwdDia->getPwd(),
                QMessageBox::Cancel);

            //sysval->pwd = "";
            //sysval->userID = -1;
            reject();
        }
        else
        {
            //sysval->user = (static_cast<MyPushButton*>(this->sender()))->text();
            //sysval->user = sysval->userMap[tmp].at(0);
            sysval->userID = tmp;
            accept();
        }
    }
}
