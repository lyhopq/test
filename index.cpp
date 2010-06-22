#include "index.h"
#include "ui_index.h"

#include <QTime>
#include <QSettings>
#include <QDebug>
//#include <QStatusBar>

#include "tobacco.cpp"
#include "userdialog.h"
#include "pwddialog.h"
#include "kbdialog.h"
#include "modifydialog.h"
#include "digitdialog.h"

// 全局变量
userDialog *userDia;
pwdDialog *pwdDia;
//MyDB *dm;
sysVal *sysval;
KBDialog *kbDia;
//modifyDialog *modifyDia;
digitDialog *digitDia;


#include "myknob.h"
#include "mypb.h"
#include "mylabel.h"
#include "timelabel.h"
#include "mysqltableview.h"
#include "myplot.h"
#include "mystatus.h"

Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);

    ui->mainStack->setCurrentIndex(0);
    ui->commandStack->setCurrentIndex(0);


    sysval  = new sysVal();
    /***
    if((sysval->isDBok = MyDB::createConn()) == true)
    {
        dm  = new MyDB();
        sysval->getUser();
    }
    ***/

    userDia = new userDialog(this);
    pwdDia  = new pwdDialog(this);
    kbDia   = new KBDialog(this);
    //modifyDia = new modifyDialog(this);
    digitDia = new digitDialog("0", "9999",this);
    digitDia->setType(0);

    this->updateUser();    // 用户列表

    //statusBar()->addWidget(ui->msgLabel);

    //QStatusBar *statusBar = new QStatusBar(this);
    //ui->statusLayout->addWidget(statusBar);

    connect(ui->backButton2, SIGNAL(clicked()), this, SLOT(on_backButton1_clicked()));
    connect(ui->backButton5, SIGNAL(clicked()), this, SLOT(on_backButton1_clicked()));
    connect(ui->backButton7, SIGNAL(clicked()), this, SLOT(on_backButton1_clicked()));
    connect(ui->backButton9, SIGNAL(clicked()), this, SLOT(on_backButton1_clicked()));
    connect(ui->backButton10, SIGNAL(clicked()), this, SLOT(on_backButton1_clicked()));
    connect(ui->backButton11, SIGNAL(clicked()), this, SLOT(on_backButton1_clicked()));



    ui->lcdNumber_sd->display(QString("00000")); // 机器速度
    ui->lcdNumber_zl->display(QString("000"));   // 烟支重量

    // 用户管理
    sID = -1;

    setGNSZ(); // 功能设置
    setCSSD(); // 参数设定
    setZLBG(); // 重量报告
    setBCSZ(); // 班次设置
    setSCSJ(); // 生产数据
    setYXJS(); // 运行监视
    setYXZT(); // 运行状态
    setXTSZ(); // 系统设置
}

Index::~Index()
{
    //sysval->saveSettings();
    delete sysval;
    delete ui;
}

void Index::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//*************************************************************************************************//
/* 用户管理
 *
 */

// 更新用户
void Index::updateUser()
{
    c = 0; // 设置当前选择位置为0---> 及第一个用户

    //if(sysval->dirty & 0x10)
    {
        QLayout *layout = ui->groupBox_user->layout();
        if(layout != NULL)
        {
            QLayoutItem *child;
            while ((child = layout->takeAt(0)) != 0)
            {
                //qDebug() << "i am here!";
                child->widget()->setParent(NULL);
                delete child;
            }

            //layout->setParent(NULL);
            //delete layout;
        }
        else
            layout = new QVBoxLayout;

        QMap<int, QStringList>::const_iterator i = sysval->userMap.constBegin();

        int n = 0;
        MyPushButton *myPb[sysval->userCount];
        while (i != sysval->userMap.constEnd())
        {
            myPb[n] = new MyPushButton(i.value().at(0), i.key());
            connect(myPb[n], SIGNAL(clicked()), this, SLOT(userInfo()));

            layout->addWidget(myPb[n]);

            if( n > 9)
                myPb[n]->hide();

            ++i;++n;
        }

        if(sysval->userCount < 9)
        {
            for(int j = 0; j< 9 - sysval->userCount; j++)
            {
                layout->addWidget(new QPushButton);
            }
        }
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        ui->groupBox_user->setLayout(layout);

        //buttonEnable();


        //sysval->dirty &= 0x01;

    }
    userButtonEnable();

    update();
}


void Index::on_pb_per_clicked()
{
    ui->groupBox_user->layout()->itemAt(c+9)->widget()->hide();
    ui->groupBox_user->layout()->itemAt(--c)->widget()->show();
    userButtonEnable();
}

void Index::on_pb_next_clicked()
{
    ui->groupBox_user->layout()->itemAt(c+10)->widget()->show();
    ui->groupBox_user->layout()->itemAt(c++)->widget()->hide();
    userButtonEnable();
}

void Index::userButtonEnable()
{

    if (c == 0)
        ui->pb_per->setEnabled(false);
    else
        ui->pb_per->setEnabled(true);

    if (c + 9 < sysval->userCount)
        ui->pb_next->setEnabled(true);
    else
        ui->pb_next->setEnabled(false);
}

// 获得用户信息
void Index::userInfo()
{
    ui->pb_userCancel->setEnabled(true);
    ui->pb_userSave->setDisabled(true);
    ui->userdelPb->setEnabled(true);
    sID = static_cast<MyPushButton*>(this->sender())->id;

    ui->lineEdit_name->setText(sysval->userMap[sID][0]);
    //ui->lineEdit_pwd->setText(sysval->userMap[tmp][1]);
    ui->lineEdit_pri->setText(sysval->userMap[sID][2]);
    ui->lineEdit_time->setText(sysval->userMap[sID][3]);
}

// 参数设定
void Index::on_buttonGroup_buttonPressed(int n)
{
    //qDebug() << "buttongroup enable!!!" << n;
    if(sID == -1) // 没有选择用户
        QMessageBox::critical(0, tr("错误"), tr("请选择一个用户设置权限！"));
    else if(sID == 0) // 选择了管理员
        QMessageBox::information(0, tr("提示"), tr("不用为管理员设置权限！"));
    else if(ui->pb_userSave->isEnabled())
        QMessageBox::warning(0, tr("警告"), tr("当前用户信息未保存，请保存后在操作！"));
    else
    {
        ui->userStack->setCurrentIndex(1);
        ui->cansuStack->setCurrentIndex(-n-2);

        load_canshu(-n-2, ui->cansuStack->currentWidget());
    }
}

// 加载初始参数
void Index::load_canshu(int n, QWidget *w)
{
    QString cs = sysval->getCS(sID, n);

    if(cs.isEmpty())
        return;

    QList<QCheckBox *> widgets = w->findChildren<QCheckBox *>();

    for(int i = 0; i < widgets.size(); ++i)
    {
        if(cs.at(i) == '1') // 为一选中
            widgets.at(i)->setCheckState(Qt::Checked);
        else
            widgets.at(i)->setCheckState(Qt::Unchecked);
    }
}

void Index::on_pb_userSave_clicked()
{
    if(ui->lineEdit_name->text() == "")
        QMessageBox::information(0, tr("错误"),
            tr("你输入的用户名为空！"));
    else
    {
    // 更新 sysval
    QStringList tmp;
    tmp << ui->lineEdit_name->text()
        << ""
        << ui->lineEdit_pri->text()
        << ui->lineEdit_time->text();
    sID = sysval->bigID + 1; // 把新加用户设置为当前选择用户
    sysval->userHand(0, sID, tmp); // 更新系统用户信息

    // 更新数据库
    QString tmp1 = QString("insert into user values(%1,'%2','%3','%4','%5');")
        .arg(sysval->bigID).arg(tmp.at(0)).arg(tmp.at(1))
        .arg(tmp.at(2)).arg(tmp.at(3));
    sysval->dm->query.exec(tmp1);

    // 更新 userDia
    userDia->updateUser();

    //更新用户列表
    this->updateUser();

    ui->pb_userSave->setDisabled(true);
    disconnect(ui->lineEdit_name, SIGNAL(getFocus()), 0, 0);
    disconnect(ui->lineEdit_pri, SIGNAL(getFocus()), 0, 0);
    disconnect(ui->lineEdit_time, SIGNAL(getFocus()), 0, 0);
    }
}

void Index::on_pb_userCancel_clicked()
{
        sID = -1;
        ui->pb_userSave->setDisabled(true);
        ui->lineEdit_name->clear();
        ui->lineEdit_pri->setText("0");
        ui->lineEdit_time->setText("0");

        disconnect(ui->lineEdit_name, SIGNAL(getFocus()), 0, 0);
        disconnect(ui->lineEdit_pri, SIGNAL(getFocus()), 0, 0);
        disconnect(ui->lineEdit_time, SIGNAL(getFocus()), 0, 0);
}

void Index::on_useraddPb_clicked()
{
    if(ui->pb_userSave->isEnabled())
    {
        if(QMessageBox::information(0, tr("确定"),
            tr("当前新增的数据还未保存，是否保存？"),
            QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
        {
            ui->pb_userSave->click();
        }
    }
    else
    {
        ui->pb_userSave->setEnabled(true);
        ui->pb_userCancel->setEnabled(true);
        ui->lineEdit_name->clear();
        ui->lineEdit_pri->setText("0");
        ui->lineEdit_time->setText("0");

        connect(ui->lineEdit_name, SIGNAL(getFocus()), this, SLOT(lineEdit_name_getFocus()));
        connect(ui->lineEdit_pri, SIGNAL(getFocus()), this, SLOT(lineEdit_pri_getFocus()));
        connect(ui->lineEdit_time, SIGNAL(getFocus()), this, SLOT(lineEdit_time_getFocus()));
        if(kbDia->exec() == QDialog::Accepted)
        {
            if(sysval->userName.indexOf(kbDia->getRes()) == -1)
                ui->lineEdit_name->insert(kbDia->getRes());
            else
                QMessageBox::critical(0, tr("错误"), tr("你输入的用户名以存在！"));
        }
    }


}

void Index::on_userdelPb_clicked()
{
    if(sID == sysval->userID)
        QMessageBox::critical(0, tr("错误"), tr("当前用户以登录，不能删除！\n %1")
            .arg(sysval->userMap[sID].at(0)));
    else
    {
        if(QMessageBox::information(0, tr("确定"), tr("确定要删除用户吗？\n %1")
            .arg(sysval->userMap[sID].at(0)),
            QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok) == QMessageBox::Ok)
        {
            QStringList tmp;
            sysval->userHand(1, sID, tmp);

            sysval->dm->query.exec(QString("delete from user where id=%1;").arg(sID));

            userDia->updateUser();
            this->updateUser();

            ui->userdelPb->setDisabled(true);
        }
    }
}

void Index::on_buttonGroupQX_buttonPressed(int n)
{
    QPushButton * pb = (QPushButton *)ui->buttonGroupQX->button(n);

    QList<QCheckBox *> widgets = pb->parent()->findChildren<QCheckBox *>(); // 得到所有的 QCheckBox

    for(int i = 0; i < widgets.size(); ++i)
    {
        widgets.at(i)->setCheckState(Qt::Checked);
    }
}

void Index::on_buttonGroupQCX_buttonPressed(int n)
{
    QPushButton * pb = (QPushButton *)ui->buttonGroupQX->button(n);

    QList<QCheckBox *> widgets = pb->parent()->findChildren<QCheckBox *>(); // 得到所有的 QCheckBox

    for(int i = 0; i < widgets.size(); ++i)
    {
        widgets.at(i)->setCheckState(Qt::Unchecked);
    }
}

void Index::on_pb_ok_clicked()
{
    ui->userStack->setCurrentIndex(0);
    //...

    // 保存用户参数设置
    QWidget *w = ui->cansuStack->currentWidget();
    int n = ui->cansuStack->currentIndex(); //由 0 开始

    save_canshu(n, w);
}

void Index::save_canshu(int n, QWidget *w)
{
    QList<QCheckBox *> widgets = w->findChildren<QCheckBox *>();
    QString cs;

    for(int i = 0; i < widgets.size(); ++i)
    {
        if(widgets.at(i)->isChecked())
            cs.append('1');
        else
            cs.append('0');
    }
    
    sysval->saveCS(sID, n, cs);
}

void Index::on_pb_cancel_clicked()
{
    ui->userStack->setCurrentIndex(0);
}

// END： 用户管理
//*************************************************************************************************//

void Index::on_syssetButton_clicked()
{
    ui->mainStack->setCurrentIndex(1);
    ui->commandStack->setCurrentIndex(1);
    ui->exitButton->setDisabled(true);

    //setXTSZ();
}

// 重量控制
void Index::on_gravityButton_clicked()
{
    //ui->mainStack->setCurrentIndex(4);
    ui->commandStack->setCurrentIndex(3);
    ui->exitButton->setDisabled(true);

    //ui->pb_gn->toggle();
    ui->pb_gn->click();
}

void Index::on_statusButton_clicked()
{
    ui->mainStack->setCurrentIndex(16);
    ui->commandStack->setCurrentIndex(6);
    ui->exitButton->setDisabled(true);
}

// 生产数据
void Index::on_dataButton_clicked()
{
    //ui->mainStack->setCurrentIndex(17);
    ui->commandStack->setCurrentIndex(7);
    ui->exitButton->setDisabled(true);

    //ui->pb_cl->toggle();
    ui->pb_cl->click();
}

void Index::on_supportButton_clicked()
{
    ui->mainStack->setCurrentIndex(24);
    ui->commandStack->setCurrentIndex(8);
    ui->exitButton->setDisabled(true);
}


void Index::on_loginButton_clicked()
{
    if(userDia->exec() == QDialog::Accepted)
    {
        ui->userLabel->setText(tr("登录用户： %1")
            .arg(sysval->userMap[sysval->userID].at(0)));

        ui->nameEdit->setText(sysval->userMap[sysval->userID].at(0));
        ui->timeEdit->setText(QTime::currentTime().toString("H:m:s"));

        ui->logoutButton->setEnabled(true);
    }
}

void Index::on_logoutButton_clicked()
{
    sysval->userID = -1;
    ui->userLabel->setText(tr("登录用户： "));
    ui->logoutButton->setDisabled(true);

    QMessageBox::information(0, tr("确认"), tr("取消登录成功！"));
}

void Index::on_exitButton_clicked()
{
    sysval->saveSettings();

    close();
}



void Index::on_pb_bc_clicked()
{
    ui->mainStack->setCurrentIndex(2);
    ui->commandStack->setCurrentIndex(2);

    sysval->xq = QDate::currentDate().dayOfWeek();
    //qDebug() << sysval->xq;
    //ui->buttonGroupXQ->button(-1-sysval->xq)->toggle();
    ui->buttonGroupXQ->button(-1-sysval->xq)->click();
}

void Index::on_pb_yh_clicked()
{
    if(sysval->userID != 0)     // 不是管理员
        QMessageBox::information(0, tr("警告"), tr("只有管理员才有权配置用户！"));
    else
    {
        ui->mainStack->setCurrentIndex(3);
        ui->userStack->setCurrentIndex(0);
        ui->command->hide();
    }
}

void Index::on_pb_xg_clicked()
{
    if(sysval->userID == -1)
        QMessageBox::information(0, tr("错误"), tr("还没有登录用户，不能修改口令！"),
            QMessageBox::Ok);
    else
    {
        modifyDialog dlg;
        dlg.exec();
    }
}

void Index::on_pb_hb_clicked()
{
    if(QMessageBox::information(0, tr("确定"),
        tr("确定要手动换班吗？"), QMessageBox::No | QMessageBox::Yes,
        QMessageBox::Yes) == QMessageBox::Yes)
    {
        //...
        QMessageBox::information(0, tr("成功"),
            tr("手动换班成功！"), QMessageBox::Ok);
    }
}



void Index::on_pb_close1_clicked()
{
    ui->mainStack->setCurrentIndex(1);
    ui->command->show();
}


void Index::on_backButton1_clicked()
{
    ui->mainStack->setCurrentIndex(0);
    ui->commandStack->setCurrentIndex(0);
    ui->exitButton->setEnabled(true);

    // 保存当前班次设置
    saveBC(currNum - 1);

    // 更新运行班次
    if(sysval->dirty.testBit(1)) // dirty 1 为 true 需更新运行班次
    {
        updateBC();
        seek_curr_banci();
        dis_note_last_banci();
        note_curr_banci();

        sysval->dirty.clearBit(1);
    }
}


void Index::on_backButton3_clicked()
{
    ui->mainStack->setCurrentIndex(1);
    ui->commandStack->setCurrentIndex(1);

    // 保存当前班次设置
    saveBC(currNum - 1);

    // 更新运行班次
    if(sysval->dirty.testBit(1)) // dirty 1 为 true 需更新运行班次
    {
        updateBC();
        seek_curr_banci();
        dis_note_last_banci();
        note_curr_banci();

        sysval->dirty.clearBit(1);
    }
}

void Index::on_backButton4_clicked()
{
    ui->mainStack->setCurrentIndex(0);
    ui->commandStack->setCurrentIndex(0);
    ui->exitButton->setEnabled(true);
}

void Index::on_backButton6_clicked()
{
    //ui->mainStack->setCurrentIndex(4);
    ui->commandStack->setCurrentIndex(3);
    //ui->pb_gn->toggle();
    ui->pb_gn->click();
}

void Index::on_backButton8_clicked()
{
    ui->mainStack->setCurrentIndex(4);
    ui->commandStack->setCurrentIndex(3);
}


void Index::on_nameEdit_getFocus()
{
    if (userDia->exec() == QDialog::Accepted)
    {
        //ui->nameEdit->clear();
        ui->nameEdit->setText(sysval->userMap[sysval->userID].at(0));
        //ui->timeEdit->clear();
        ui->timeEdit->setText(QTime::currentTime().toString("H:m:s"));

        ui->userLabel->setText(tr("登录用户： %1").arg(ui->nameEdit->text()));
    }
}





void Index::on_pb_ok2_clicked()
{
    ui->mainStack->setCurrentIndex(11);
}

void Index::on_pb_cancel2_clicked()
{
    ui->mainStack->setCurrentIndex(11);
}


void Index::on_buttonGroup2_buttonPressed(int n)
{
    //qDebug() << "buttongroup enable!!!" << n;
    ui->mainStack->setCurrentIndex(n+8);
}

void Index::on_buttonGroup3_buttonPressed(int n)
{
    ui->mainStack->setCurrentIndex(-n+5);
}

void Index::on_buttonGroup4_buttonPressed(int n)
{
    ui->mainStack->setCurrentIndex(-n+11);
}

void Index::on_buttonGroup5_buttonPressed(int n)
{
    ui->mainStack->setCurrentIndex(-n+15);

    if(n == -8)
        //ui->pbBB->toggle();
        ui->pbBB->click();
}

void Index::on_buttonGroup6_buttonPressed(int n)
{
    ui->mainStack->setCurrentIndex(-n+22);
}

void Index::on_buttonGroupTZ_buttonPressed(int n)
{
    //...
}




void Index::on_pb_js_clicked()
{
    //ui->mainStack->setCurrentIndex(7);
    ui->commandStack->setCurrentIndex(4);
    //ui->pb_csd->toggle();
    ui->pb_csd->click();
}

void Index::on_pb_cs_clicked()
{
    //ui->mainStack->setCurrentIndex(13);
    ui->commandStack->setCurrentIndex(5);
    //ui->pb_zl->toggle();
    ui->pb_zl->click();

    this->changeStatus();
}

void Index::on_pb_ctrl_clicked()
{
    ui->mainStack->setCurrentIndex(12);
}

void Index::on_pb_tl_clicked()
{
    //digitDialog d("0.1", "3.0", 1);
    digitDia->setType(1);
    digitDia->changeLabel("0.1", "3.0");
    //if(d.exec() == QDialog::Accepted)
    if(digitDia->exec() == QDialog::Accepted)
    {
        double d = digitDia->getRes().toDouble();
        if(d < 0.1)
            QMessageBox::critical(0, tr("错误"), tr("你输入的值不能小于 0.1！"));
        else if(d > 3.0)
            QMessageBox::critical(0, tr("错误"), tr("你输入的值不能大于 3.0！"));
        else
            ui->label_dqz->setText(tr("当前值： %1").arg(d));
    }
}


void Index::lineEdit_name_getFocus()
{
    if(kbDia->exec() == QDialog::Accepted)
        ui->lineEdit_name->setText(kbDia->getRes());
}

void Index::lineEdit_pri_getFocus()
{
    if(digitDia->exec() == QDialog::Accepted)
        ui->lineEdit_pri->setText(digitDia->getRes());
}

void Index::lineEdit_time_getFocus()
{
    if(digitDia->exec() == QDialog::Accepted)
        ui->lineEdit_time->setText(digitDia->getRes());

}
















//*************************************************************************************************//
/* 功能设置
 *
 */
void Index::setGNSZ()
{
    QGridLayout *layout1 = (QGridLayout *)ui->frame1->layout();
    layout1->setColumnMinimumWidth(0, 250);
    layout1->setColumnMinimumWidth(1, 200);
    layout1->setColumnMinimumWidth(2, 200);

    ui->lineEdit_paihao->setText(sysval->cfg[4]);
    ui->lineEdit_mincheng->setText(sysval->cfg[5]);

    // 内排采样
    p1 = new MyPb(ui->frame1);
    QLabel *cy1 = new QLabel(tr("内排采样"), ui->frame1);
    cy1->setAlignment(Qt::AlignCenter);
    ui->v1->addWidget(p1);
    ui->v1->addWidget(cy1);

    // 外排采样
    p2 = new MyPb(ui->frame1);
    QLabel *cy2 = new QLabel(tr("外排采样"), ui->frame1);
    cy2->setAlignment(Qt::AlignCenter);
    ui->v2->addWidget(p2);
    ui->v2->addWidget(cy2);

    // 内排剔除
    p3 = new MyPb(ui->frame1);
    QLabel *cy3 = new QLabel(tr("内排剔除"), ui->frame1);
    cy3->setAlignment(Qt::AlignCenter);
    ui->v3->addWidget(p3);
    ui->v3->addWidget(cy3);

    // 外排剔除
    p4 = new MyPb(ui->frame1);
    QLabel *cy4 = new QLabel(tr("外排剔除"), ui->frame1);
    cy4->setAlignment(Qt::AlignCenter);
    ui->v4->addWidget(p4);
    ui->v4->addWidget(cy4);


    // 废品功能
    c1 = new MyKnob(tr("关"), tr("开"), ui->frame1);
    QLabel *la1 = new QLabel(tr("废品功能"), ui->frame1);
    la1->setAlignment(Qt::AlignCenter);
    ui->l1->addWidget(c1);
    ui->l1->addWidget(la1);
    if(sysval->cfg[0] == "关")
        c1->setAngle(false);
    else
        c1->setAngle(true);
    connect(c1, SIGNAL(clicked()), this, SLOT(clClicked()));

    // 重量越限停机
    c2 = new MyKnob(tr("关"), tr("开"), ui->frame1);
    QLabel *la2 = new QLabel(tr("重量越限停机"), ui->frame1);
    la2->setAlignment(Qt::AlignCenter);
    ui->l2->addWidget(c2);
    ui->l2->addWidget(la2);
    if(sysval->cfg[1] == "关")
        c2->setAngle(false);
    else
        c2->setAngle(true);
    connect(c2, SIGNAL(clicked()), this, SLOT(c2Clicked()));

    bool b1 = false, b2 = false;
    // 重量控制模式
    c3 = new MyKnob(tr("手动"), tr("自动"), ui->frame1);
    QLabel *la3 = new QLabel(tr("重量控制模式"), ui->frame1);
    la3->setAlignment(Qt::AlignCenter);
    ui->l3->addWidget(c3);
    ui->l3->addWidget(la3);
    if(sysval->cfg[2] == "手动")
    {
        c3->setAngle(false);
        b1 = true;
    }
    else
        c3->setAngle(true);
    connect(c3, SIGNAL(clicked()), this, SLOT(c3Clicked()));

    // 紧头控制模式
    c4 = new MyKnob(tr("手动"), tr("自动"), ui->frame1);
    QLabel *la4 = new QLabel(tr("紧头控制模式"), ui->frame1);
    la4->setAlignment(Qt::AlignCenter);
    ui->l4->addWidget(c4);
    ui->l4->addWidget(la4);
    if(sysval->cfg[3] == "手动")
    {
        c4->setAngle(false);
        b2 = true;
    }
    else
        c4->setAngle(true);
    connect(c4, SIGNAL(clicked()), this, SLOT(c4Clicked()));

    // 劈刀调整
    ui->left->setDisabled(b1);
    ui->right->setDisabled(b1);
    ui->up->setDisabled(b2);
    ui->down->setDisabled(b2);
}

void Index::clClicked()
{
    if(sysval->cfg[0] == "关")
    {
        if(QMessageBox::information(0, tr("提示"),
            tr("废品功能投入使用？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[0] = "开";
            c1->setAngle(true);
        }

   }
   else
   {
         if(QMessageBox::information(0, tr("提示"),
            tr("关闭废品功能？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[0] = "关";
            c1->setAngle(false);
        }
   }
}

void Index::c2Clicked()
{
    if(sysval->cfg[1] == "关")
    {
        if(QMessageBox::information(0, tr("提示"),
            tr("允许重量越限后停机？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[1] = "开";
            c2->setAngle(true);
        }

   }
   else
   {
         if(QMessageBox::information(0, tr("提示"),
            tr("不允许重量越限后停机？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[1] = "关";
            c2->setAngle(false);
        }
   }
}

void Index::c3Clicked()
{
    if(sysval->cfg[2] == "手动")
    {
        if(QMessageBox::information(0, tr("提示"),
            tr("自动重量控制？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[2] = "自动";
            c3->setAngle(true);

            ui->left->setDisabled(true);
            ui->right->setDisabled(true);
        }

   }
   else
   {
         if(QMessageBox::information(0, tr("提示"),
            tr("手动重量控制？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[2] = "手动";
            c3->setAngle(false);

            ui->left->setEnabled(true);
            ui->right->setEnabled(true);
        }
   }
}

void Index::c4Clicked()
{
    if(sysval->cfg[3] == "手动")
    {
        if(QMessageBox::information(0, tr("提示"),
            tr("自动紧头控制？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[3] = "自动";
            c4->setAngle(true);

            ui->up->setDisabled(true);
            ui->down->setDisabled(true);
        }

   }
   else
   {
         if(QMessageBox::information(0, tr("提示"),
            tr("手动紧头控制？"), QMessageBox::Cancel | QMessageBox::Ok,
            QMessageBox::Ok) == QMessageBox::Ok)
        {
            sysval->cfg[3] = "手动";
            c4->setAngle(false);

            ui->up->setEnabled(true);
            ui->down->setEnabled(true);
        }
   }
}

// END： 功能设置
//*************************************************************************************************//


//*************************************************************************************************//
/* 参数设定
 *
 */

void Index::setCSSD()
{
    // 品牌参数
    mylabel[0] = new MyLabel(tr("1、目标重量"), tr("??"), tr("mg"));
    mylabel[1] = new MyLabel(tr("2、重量调整"), tr(""));
    mylabel[2] = new MyLabel(tr("3、校准调整"), tr("??"));
    mylabel[3] = new MyLabel(tr("4、校准斜率"), tr("??"), tr("mg"));
    mylabel[4] = new MyLabel(tr("5、废品重量下限"), tr("??"), tr("mg"));
    mylabel[5] = new MyLabel(tr("6、废品重量上限"), tr("??"), tr("mg"));
    mylabel[6] = new MyLabel(tr("7、软点限度"), tr("??"), tr("%"));
    mylabel[7] = new MyLabel(tr("8、硬点限度"), tr("??"), tr("%"));
    mylabel[8] = new MyLabel(tr("9、轻烟端限度"), tr("??"), tr("%"));
    mylabel[9] = new MyLabel(tr("10、启动位置"), tr("??"));
    mylabel[10] = new MyLabel(tr("11、增量调整"), tr("??"));
    mylabel[11] = new MyLabel(tr("12、至修整器的烟条数"), tr("??"));
    mylabel[12] = new MyLabel(tr("13、内排剔除步距"), tr("??"));
    mylabel[13] = new MyLabel(tr("14、废品烟条残余增量"), tr("??"));
    mylabel[14] = new MyLabel(tr("15、外排剔除步距"), tr("??"));
    mylabel[15] = new MyLabel(tr("16、烟条长度"), tr("??"), tr("mm"));
    
    QGridLayout *layout2 = new QGridLayout(ui->framePPCU);
    for(int i = 0; i < 16; i++)
    {
        layout2->addWidget(mylabel[i], i%8, i < 8 ? 0:1);
        mylabel[i]->setNum(i);
        mylabel[i]->setV(sysval->cfg[i+10]);
        connect(mylabel[i], SIGNAL(clicked(int)), this, SLOT(clicked(int)));
    }


    // 重量参数
    mylabel[16] = new MyLabel(tr("1、废品门运行时间"), tr("??"));
    mylabel[17] = new MyLabel(tr("2、控制阀值"), tr("??"));
    mylabel[18] = new MyLabel(tr("3、目标重量偏移烟条数"), tr("??"));
    mylabel[19] = new MyLabel(tr("4、修整器滞后"), tr("??"));
    mylabel[20] = new MyLabel(tr("5、扫描器噪声补偿"), tr("??"));
    mylabel[21] = new MyLabel(tr("6、重量偏差重限"), tr("??"));
    mylabel[22] = new MyLabel(tr("7、重量偏差轻限"), tr("??"));
    mylabel[23] = new MyLabel(tr("8、紧头控制滞后"), tr("??"));
    mylabel[24] = new MyLabel(tr("9、紧头自动调整步幅"), tr("??"));
    mylabel[25] = new MyLabel(tr("10、紧头调整用烟条数"), tr("??"));
    mylabel[26] = new MyLabel(tr("11、紧头位置偏差极限"), tr("??"));
    mylabel[27] = new MyLabel(tr("12、紧头手动调整步幅"), tr("??"));
    mylabel[28] = new MyLabel(tr("13、重量控制比例系数"), tr("??"));
    mylabel[29] = new MyLabel(tr("14、重量手动调整步幅"), tr("??"));
    mylabel[30] = new MyLabel(tr("15、紧头调整"), tr("??"));
    mylabel[31] = new MyLabel(tr("16、目标速率"));

    QGridLayout *layout3 = new QGridLayout(ui->frameZLCS);
    int j = 16;
    for(int i = 0; i < 16; i++)
    {
        layout3->addWidget(mylabel[j], i%8, i < 8 ? 0:1);
        mylabel[j]->setNum(j);
        mylabel[j]->setV(sysval->cfg[j+10]);
        connect(mylabel[j], SIGNAL(clicked(int)), this, SLOT(clicked(int)));
        j++;
    }
}

void Index::changeStatus()
{
    bool on = sysval->userID == 0 ? true : false;

    for(int i = 0; i < 32; i++)
    {
        mylabel[i]->changStatus(on);
    }
}

void Index::clicked(int num)
{
    int id = sysval->userID;

    if(id == -1)
        QMessageBox::information(0, tr("提示"), tr("你还没有登录，不能修改参数！"));
    else if(id == 0)
    {
        getVal(num);
    }
    else
        QMessageBox::information(0, tr("提示"), tr("你没有修改此参数的权限！"));
}

void Index::getVal(int num)
{
    double x1 = 0.0;
    double x2 = 0.0;
    switch(num)
    {
        case 0:
            x1 = 500; x2 = 1500;
            break;
        case 1:
            x1 = -100; x2 = 100;
            break;
        case 2:
            return;
            break;
        case 3:
            x1 = 1; x2 = 17;
            break;
        case 4:
        case 5:
        case 9:
            x1 = 0; x2 = 255;
            break;
        case 6:
            x1 = -100; x2 = -20;
            break;
        case 7:
        case 8:
            x1 = -80; x2 = 80;
            break;

        case 10:
            x1 = 0; x2 = 127;
            break;
        case 11:
            x1 = 15; x2 = 33;
            break;
        case 12:
            x1 = 0; x2 = 280;
            break;
        case 13:
            x1 = 0; x2 = 63;
            break;
        case 14:
            x1 = 80; x2 = 300;
            break;
        case 15:
            x1 = 50; x2 = 100;
            break;


        case 16:
            x1 = 1; x2 = 64;
            break;
        case 17:
            x1 = 0; x2 = 8500;
            break;
        case 18:
        case 27:
        case 29:
            x1 = 1; x2 = 10;
            break;
        case 19:
            x1 = 2; x2 = 10;
            break;
        case 20:
            x1 = -30; x2 = 30;
            break;
        case 21:
            x1 = 20; x2 = 100;
            break;
        case 22:
            x1 = -100; x2 = 20;
            break;
        case 23:
            x1 = 0.2; x2 = 3.0;
            break;
        case 24:
            x1 = 1; x2 = 100;
            break;
        case 25:
            x1 = 20; x2 = 100;
            break;
        case 26:
            x1 = 0; x2 = 128;
            break;
        case 28:
            x1 = 1; x2 = 255;
            break;
        case 30:
            x1 = 0; x2 = 127;
            break;
        case 31:
            x1 = 0; x2 = 10000;
            break;

        default:
            break;
    }

    if(num != 23)
        digitDia->setType(2);
    else
        digitDia->setType(1);

    digitDia->changeLabel(QString("%1").arg(x1), QString("%1").arg(x2));
    if(digitDia->exec() == QDialog::Accepted)
    {
        if(digitDia->isFitting())
        {
            QString v = digitDia->getRes();
            mylabel[num]->setV(v);
            sysval->cfg[10+num] = v;
            //...
        }
    }
}

// END: 参数设置
//*************************************************************************************************//


//*************************************************************************************************//
/* 班次设置
 *
 */
void Index::setBCSZ()
{
    for(int i = 0; i < 20; i++)
    {
        tLabel[i] = new TimeLabel(ui->frameBCSZ);
        tLabel[i]->setNum(i);
        if(i%5 == 0)
        {
            tLabel[i]->setColor(true, QColor(Qt::green));
            connect(tLabel[i], SIGNAL(tClicked(int)), this, SLOT(tClicked(int)));
        }
        else if(i%5 == 4)
            tLabel[i]->setColor(false);
        else
        {
            tLabel[i]->setColor();
            connect(tLabel[i], SIGNAL(tClicked(int)), this, SLOT(tClicked(int)));
        }
    }


    // 一
    ui->g11->layout()->addWidget(tLabel[0]);
    ui->g11->setFlat(true);
    ui->g12->layout()->addWidget(tLabel[1]);
    ui->g12->layout()->addWidget(tLabel[2]);
    ui->g12->layout()->addWidget(tLabel[3]);
    ui->g13->layout()->addWidget(tLabel[4]);

    // 二
    ui->g21->layout()->addWidget(tLabel[5]);
    ui->g21->setFlat(true);
    ui->g22->layout()->addWidget(tLabel[6]);
    ui->g22->layout()->addWidget(tLabel[7]);
    ui->g22->layout()->addWidget(tLabel[8]);
    ui->g23->layout()->addWidget(tLabel[9]);

    // 三
    ui->g31->layout()->addWidget(tLabel[10]);
    ui->g31->setFlat(true);
    ui->g32->layout()->addWidget(tLabel[11]);
    ui->g32->layout()->addWidget(tLabel[12]);
    ui->g32->layout()->addWidget(tLabel[13]);
    ui->g33->layout()->addWidget(tLabel[14]);

    // 四
    ui->g41->layout()->addWidget(tLabel[15]);
    ui->g41->setFlat(true);
    ui->g42->layout()->addWidget(tLabel[16]);
    ui->g42->layout()->addWidget(tLabel[17]);
    ui->g42->layout()->addWidget(tLabel[18]);
    ui->g43->layout()->addWidget(tLabel[19]);

    //currNum = sysval->xq;
    currNum = -1;
}

// timelabel clicked
void Index::tClicked(int num)
{
    digitDia->setType(0);
    digitDia->changeLabel("00:00", "23:59");

    if(digitDia->exec() == QDialog::Accepted)
    {
        if(digitDia->isFitting())
        {
            QString tmp = digitDia->getRes();
            tLabel[num]->setTime(tmp);

            sysval->dirty.setBit(0); // 需更新班次设置-->存入数据库
            sysval->dirty.setBit(1); // 需更新运行班次-->显示
        }
    }
}

void Index::on_buttonGroupXQ_buttonPressed(int n)
{
    static int lastNum = -1;  // 前一天
    //static int currNum  = -1; // 当前星期
    if(currNum == -n-1)  // 同一天不需更新
        return;


    // 星期切换
    lastNum = currNum;
    currNum = -n-1; // 更新当前星期

    saveBC(lastNum - 1);

    // 获取当前设置
    QStringList time;
    bool test;

    if((test = sysval->getBC(-n-2, time)))
    {
        int i, j=0;
        for(i = 0; i < 16; i++, j++)
        {
            if(j%5 == 4)
                j++;
            tLabel[j]->setTime(time.at(i));
        }

        //sysval->dirty.clearBit(0);; // 已更新
    }

    //qDebug() << -n-2 << test;
}

void Index::saveBC(int week)
{
    bool test;

    if(sysval->dirty.testBit(0)) // dirty 0 为 true 需保存前一天的设置
    {
        QStringList tmp;
        tmp << tLabel[0]->getTime()
            << tLabel[1]->getTime()
            << tLabel[2]->getTime()
            << tLabel[3]->getTime()
            << tLabel[5]->getTime()
            << tLabel[6]->getTime()
            << tLabel[7]->getTime()
            << tLabel[8]->getTime()
            << tLabel[10]->getTime()
            << tLabel[11]->getTime()
            << tLabel[12]->getTime()
            << tLabel[13]->getTime()
            << tLabel[15]->getTime()
            << tLabel[16]->getTime()
            << tLabel[17]->getTime()
            << tLabel[18]->getTime();

       test = sysval->saveBC(week, tmp);
       sysval->dirty.clearBit(0);

       //qDebug() << week << tmp;

       if(!test)
           qDebug() << sysval->week.at(week) << tr("保存班次设置错误，请检查数据库！");
    }

}

// END: 班次设置
//*************************************************************************************************//


//*************************************************************************************************//
/* 生产数据
 *
 */
void Index::setSCSJ()
{

    // 产量报告
    MySqlModel *mclbg = new MySqlModel;
    mclbg->setAttr(3);
    mclbg->setQuery("select * from CLBG;");
    mclbg->setHeaderData(0, Qt::Horizontal, "");
    mclbg->setHeaderData(1, Qt::Horizontal, "");
    mclbg->setHeaderData(2, Qt::Horizontal, tr("本班"));
    mclbg->setHeaderData(3, Qt::Horizontal, tr("上一班"));
    mclbg->setHeaderData(4, Qt::Horizontal, tr("再上一班"));

    MyTableView *tclbg = new MyTableView(mclbg, ui->frameCLBG);
    QLayout *layout = ui->frameCLBG->layout();
    layout->addWidget(tclbg);

    // 重量废品%
    MySqlModel *mzlfp1 = new MySqlModel;
    //mzlfp1->setAttr(3);
    mzlfp1->setQuery("select * from ZLFP1;");
    mzlfp1->setHeaderData(0, Qt::Horizontal, "");
    mzlfp1->setHeaderData(1, Qt::Horizontal, "");
    mzlfp1->setHeaderData(2, Qt::Horizontal, tr("现实"));
    mzlfp1->setHeaderData(3, Qt::Horizontal, tr("平均"));

    MyTableView *tzlfp1 = new MyTableView(mzlfp1, ui->frameZLFP1);
    layout = ui->frameZLFP1->layout();
    layout->addWidget(tzlfp1);

    // 重量废品
    MySqlModel *mzlfp2 = new MySqlModel;
    mzlfp2->setAttr(3);
    mzlfp2->setQuery("select * from ZLFP2;");
    mzlfp2->setHeaderData(0, Qt::Horizontal, "");
    mzlfp2->setHeaderData(1, Qt::Horizontal, "");
    mzlfp2->setHeaderData(2, Qt::Horizontal, tr("本班"));
    mzlfp2->setHeaderData(3, Qt::Horizontal, tr("上一班"));
    mzlfp2->setHeaderData(4, Qt::Horizontal, tr("再上一班"));

    MyTableView *tzlfp2 = new MyTableView(mzlfp2, ui->frameZLFP2);
    layout = ui->frameZLFP2->layout();
    layout->addWidget(tzlfp2);

    // 外部废品%
    MySqlModel *mwbfp1 = new MySqlModel;
    mwbfp1->setQuery("select * from WBFP;");
    mwbfp1->setHeaderData(0, Qt::Horizontal, "");
    mwbfp1->setHeaderData(1, Qt::Horizontal, "");
    mwbfp1->setHeaderData(2, Qt::Horizontal, tr("现实"));
    mwbfp1->setHeaderData(3, Qt::Horizontal, tr("平均"));

    MyTableView *twbfp1 = new MyTableView(mwbfp1, ui->frameWBFP1);
    layout = ui->frameWBFP1->layout();
    layout->addWidget(twbfp1);

    // 外部废品
    MySqlModel *mwbfp2 = new MySqlModel;
    mwbfp2->setAttr(3);
    mwbfp2->setQuery("select * from WBFP2;");
    mwbfp2->setHeaderData(0, Qt::Horizontal, "");
    mwbfp2->setHeaderData(1, Qt::Horizontal, "");
    mwbfp2->setHeaderData(2, Qt::Horizontal, tr("本班"));
    mwbfp2->setHeaderData(3, Qt::Horizontal, tr("上一班"));
    mwbfp2->setHeaderData(4, Qt::Horizontal, tr("再上一班"));

    MyTableView *twbfp2 = new MyTableView(mwbfp2, ui->frameWBFP2);
    layout = ui->frameWBFP2->layout();
    layout->addWidget(twbfp2);

    // 停机记录
    QSqlQueryModel *mtjjl = new QSqlQueryModel;
    mtjjl->setQuery("select * from TJJL;");
    mtjjl->setHeaderData(0, Qt::Horizontal, tr("序号"));
    mtjjl->setHeaderData(1, Qt::Horizontal, tr("起始时间"));
    mtjjl->setHeaderData(2, Qt::Horizontal, tr("结束时间"));
    mtjjl->setHeaderData(3, Qt::Horizontal, tr("停机原因"));


    QTableView *ttjjl = new QTableView(ui->frameTJJL);
    QHeaderView *hv = ttjjl->horizontalHeader();
    //hv->setResizeMode(QHeaderView::Stretch);
    hv->setResizeMode(QHeaderView::Fixed);
    hv->setStretchLastSection(true);

    hv = ttjjl->verticalHeader();
    hv->setHidden(true);

    ttjjl->setModel(mtjjl);
    layout = ui->frameTJJL->layout();
    layout->addWidget(ttjjl);

    ttjjl->setColumnWidth(0,60);
    ttjjl->setColumnWidth(1,100);
    ttjjl->setColumnWidth(2,100);

    ttjjl->setSelectionBehavior(QAbstractItemView::SelectRows);
    ttjjl->setCurrentIndex(mtjjl->index(0,0));


    // 停机分析
    // 本班
    QSqlQueryModel *mtjfx1 = new QSqlQueryModel;
    MyTableView1 *ttjfx1 = new MyTableView1(mtjfx1, "TJFX1", ui->frameBB);

    layout = ui->frameBB->layout();
    layout->addWidget(ttjfx1);

    // 上一班
    QSqlQueryModel *mtjfx2 = new QSqlQueryModel;
    MyTableView1 *ttjfx2 = new MyTableView1(mtjfx2, "TJFX2", ui->frameSYB);

    layout = ui->frameSYB->layout();
    layout->addWidget(ttjfx2);

    // 再上一班
    QSqlQueryModel *mtjfx3 = new QSqlQueryModel;
    MyTableView1 *ttjfx3 = new MyTableView1(mtjfx3, "TJFX3", ui->frameZSYB);

    layout = ui->frameZSYB->layout();
    layout->addWidget(ttjfx3);
}

void Index::on_buttonGroupTJFX_buttonPressed(int n)
{
    ui->stackedTJFX->setCurrentIndex(-n-2);
}

// END： 生产数据
//*************************************************************************************************//


//*************************************************************************************************//
/* 重量报告
 *
 */
void Index::setZLBG()
{
    MySqlModel *mzlbg = new MySqlModel;
    mzlbg->setQuery("select * from ZLBG;");
    mzlbg->setHeaderData(0, Qt::Horizontal, "");
    mzlbg->setHeaderData(1, Qt::Horizontal, "");
    mzlbg->setHeaderData(2, Qt::Horizontal, tr("现实"));
    mzlbg->setHeaderData(3, Qt::Horizontal, tr("平均"));

    MyTableView *tzlbg = new MyTableView(mzlbg, ui->frameZLBG);

    QLayout *layout = ui->frameZLBG->layout();
    layout->addWidget(tzlbg);
}

// END: 重量报告
//*************************************************************************************************//


//*************************************************************************************************//
/* 运行监视
 *
 */
void Index::setYXJS()
{
    // 测试点
    ui->labelYTJC->setPixmap(QPixmap(":/sources/led-red.png"));
    ui->labelKM->setPixmap(QPixmap(":/sources/led-red.png"));

    ui->labelZSMC->setPixmap(QPixmap(":/sources/off.png"));
    ui->labelDDJ->setText(tr("不动"));


    // 紧头监视
    Plot1 *plot1 = new Plot1(ui->frameJTJS);

    slider = new QwtSlider(ui->frameJTJS, Qt::Horizontal, QwtSlider::NoScale);
    slider->setReadOnly(true);
    slider->setRange(-64, 64);
    slider->setThumbLength(40);
    slider->setThumbWidth(7);
    slider->setBorderWidth(1);
    slider->setBgStyle(QwtSlider::BgSlot);
    slider->setFocusPolicy(Qt::NoFocus);

    QLabel *l1 = new QLabel(tr("压实端位置：<font color=red>#</font>"), ui->frameJTJS);
    QLabel *l2 = new QLabel(tr("压实量：<font color=red>#</font>"), ui->frameJTJS);
    QLabel *l3 = new QLabel("mm", ui->frameJTJS);
    QLabel *l4 = new QLabel("%", ui->frameJTJS);

    v1 = new QLabel(ui->frameJTJS);
    v2 = new QLabel(ui->frameJTJS);
    v1->setFixedWidth(60);
    v2->setFixedWidth(60);
    v1->setAlignment(Qt::AlignCenter);
    v2->setAlignment(Qt::AlignCenter);

    QHBoxLayout *h1 = new QHBoxLayout;
    h1->addStretch(1);
    h1->addWidget(l1);
    h1->addWidget(v1);
    h1->addWidget(l3);
    h1->addSpacing(150);
    h1->addWidget(l2);
    h1->addWidget(v2);
    h1->addWidget(l4);
    h1->addStretch(1);

    QLayout *layout = ui->frameJTJS->layout();
    layout->addWidget(plot1);
    layout->addWidget(slider);
    layout->addItem(h1);


    // 重量监视
    plot2 = new Plot2(ui->frameZLJS);

    b20 = new QPushButton("20", ui->frameZLJS);
    b50 = new QPushButton("50", ui->frameZLJS);
    b100 = new QPushButton("100", ui->frameZLJS);

    b20->setFocusPolicy(Qt::NoFocus);
    b50->setFocusPolicy(Qt::NoFocus);
    b100->setFocusPolicy(Qt::NoFocus);
    b20->setFocusPolicy(Qt::NoFocus);
    b20->setCheckable(true);
    b50->setCheckable(true);
    b100->setCheckable(true);

    b20->setChecked(true);

    bg = new QButtonGroup(ui->frameZLJS);
    bg->addButton(b20,0);
    bg->addButton(b50,1);
    bg->addButton(b100,2);

    t1 = new QLabel(ui->frameZLJS);
    t2 = new QLabel(ui->frameZLJS);

    t1->setObjectName("t1");
    t2->setObjectName("t2");

    //qDebug() << t1->objectName() << t2->objectName();

    QLabel *l5 = new QLabel(tr("内排平均重量：<font color=red>#</font>"), ui->frameZLJS);
    QLabel *l6 = new QLabel(tr("外排平均重量：<font color=red>#</font>"), ui->frameZLJS);
    QLabel *l7 = new QLabel("mg", ui->frameZLJS);
    QLabel *l8 = new QLabel("mg", ui->frameZLJS);


    v3 = new QLabel(ui->frameZLJS);
    v4 = new QLabel(ui->frameZLJS);
    v3->setFixedWidth(60);
    v4->setFixedWidth(60);
    v3->setAlignment(Qt::AlignCenter);
    v4->setAlignment(Qt::AlignCenter);



    QHBoxLayout *h2 = new QHBoxLayout;
    h2->addWidget(b20);
    h2->addWidget(b50);
    h2->addWidget(b100);
    h2->addSpacing(50);
    h2->addWidget(t1);
    h2->addSpacing(5);
    h2->addWidget(l5);
    h2->addWidget(v3);
    h2->addWidget(l7);
    h2->addSpacing(100);
    h2->addWidget(t2);
    h2->addSpacing(5);
    h2->addWidget(l6);
    h2->addWidget(v4);
    h2->addWidget(l8);
    h2->addStretch(1);

    QLayout *layout2 = ui->frameZLJS->layout();
    layout2->addWidget(plot2);
    layout2->addItem(h2);

    connect(bg, SIGNAL(buttonClicked(int)), this, SLOT(onbg_buttonPressed(int)));


    // 实时重量
    Plot3 *plot3 = new Plot3(ui->frameSSZL);
    layout = ui->frameSSZL->layout();
    layout->addWidget(plot3);

}

void Index::onbg_buttonPressed(int n)
{
    int x = 0, d = 0;
    switch(n)
    {
        case 0:
            x = 20;
            d = 4;
            break;
        case 1:
            x = 50;
            d = 10;
            break;
        case 2:
            x = 100;
            d = 20;
            break;
    }

    //qDebug() << n << x << d;

    plot2->changScale(x, d);
}

// END: 运行监视
//*************************************************************************************************//


//*************************************************************************************************//
/* 运行状态
 *
 */
void Index::setYXZT()
{
    MyStatus *status = new MyStatus(ui->frameBG);
    QLayout *layout = ui->frameBG->layout();
    layout->addWidget(status);

    MySqlModel *myxzt = new MySqlModel;
    myxzt->setQuery("select d1,d2 from YXZT order by id;");

    myxzt->setHeaderData(0, Qt::Horizontal, tr("标准偏差 mg"));
    myxzt->setHeaderData(1, Qt::Horizontal, tr("废品率 %"));


    MyTableView *tyxzt = new MyTableView(myxzt, ui->frameDB);
    layout = ui->frameDB->layout();
    layout->addWidget(tyxzt);


    ui->Knob1->setRange(-50, 50, 0, 1);
    ui->Knob2->setRange(-10, 10, 0, 1);
    ui->Knob1->setScaleMaxMajor(10);
    ui->Knob2->setScaleMaxMajor(10);

    ui->Knob1->setFocusPolicy(Qt::NoFocus);
    ui->Knob2->setFocusPolicy(Qt::NoFocus);
}
// END: 运行状态
//*************************************************************************************************//


//*************************************************************************************************//
/* 系统设置
 *
 * TODO:
 * //1. 如何进行星期切换---> 取决于班次的设置
 * 2. 暂停时间相关
 */
void Index::setXTSZ()
{
    c_banci = 0;

    bc_timer = new QTimer(this);
    connect(bc_timer, SIGNAL(timeout()), this, SLOT(bc_timer_timerout())); // 班次定时

    updateBC();

    seek_curr_banci();
    note_curr_banci();

    //bc_timer->setSingleShot(false);
    //bc_timer->setInterval(time_map[c_banci]);
    bc_timer->start();
    //qDebug() << curr << t1 << t2 << curr.msecsTo(t2);
}

// 寻找当前班次
void Index::seek_curr_banci()
{
    QTime curr = QTime::currentTime();
    QTime t1, t2;
    QStringList tmp;
    for(int i = 0; i != 4; ++i)
    {
        tmp = bc_list.at(i*4).split("-");
        if(tmp.size() != 2)
        {
            qDebug() << tr("班次设置的时间格式不对,请检查！");
            c_banci = l_banci = 0;
            return;
        }
        t1 = QTime::fromString(tmp.at(0), "hh:mm");
        t2 = QTime::fromString(tmp.at(1), "hh:mm");

        //time_map[i] = t1.msecsTo(t2);

        if(curr >= t1 && curr <= t2)
        {
            l_banci = c_banci; // 记住上一班
            c_banci  = i;
            //time_map[c_banci] = curr.msecsTo(t2);
            bc_timer->setInterval(curr.msecsTo(t2));
            break; // 找到
        }
    }

}

// 更新班次显示
void Index::updateBC()
{
    if(c_banci == 0)  // c_banci == 0 ---> 新的一天开始
        sysval->xq = QDate::currentDate().dayOfWeek();

    bool test = sysval->getBC(sysval->xq - 1, bc_list);
    if(!test)
        return;
    display_bc(bc_list);
    //qDebug() << test << bc_list;
}

// 高亮当前班次
void Index::note_curr_banci()
{
    switch(c_banci)
    {
        case 0:
            ui->label_1b->setStyleSheet("color: red");
            ui->b1Label->setStyleSheet("color: red");
            break;
        case 1:
            ui->label_2b->setStyleSheet("color: red");
            ui->b2Label->setStyleSheet("color: red");
            break;
        case 2:
            ui->label_3b->setStyleSheet("color: red");
            ui->b3Label->setStyleSheet("color: red");
            break;
        case 3:
            ui->label_4b->setStyleSheet("color: red");
            ui->b4Label->setStyleSheet("color: red");
            break;
        default:
            break;
    }
}

// 去掉上一次班次高亮，并且设定下一次换班 timer
void Index::dis_note_last_banci()
{
    QLabel *l1;
    QLabel *l2;
    QString tmp;
    switch(l_banci)
    {
        case 3:
            l1 = ui->label_4b;
            l2 = ui->b4Label;
            break;
        case 0:
            l1 = ui->label_1b;
            l2 = ui->b1Label;
            break;
        case 1:
            l1 = ui->label_2b;
            l2 = ui->b2Label;
            break;
        case 2:
            l1 = ui->label_3b;
            l2 = ui->b3Label;
            break;
        default:
            return;
            break;
    }
    l1->setStyleSheet("color: blue");
    l2->setStyleSheet("color: green");

    // 得到当前班次时间数据
    switch(c_banci)
    {
        case 0:
            tmp = ui->b1Label->text();
            break;
        case 1:
            tmp = ui->b2Label->text();
            break;
        case 2:
             tmp = ui->b3Label->text();
            break;
        case 3:
             tmp = ui->b4Label->text();
            break;
    }

    // 计算下一次换班
    QTime t1, t2;
    QStringList list;
    list = tmp.split("-");
    if(tmp.size() != 2)
    {
        qDebug() << tr("班次设置的时间格式不对,请检查！");
        return;
    }

    t1 = QTime::fromString(list.at(0), "hh:mm");
    t2 = QTime::fromString(list.at(1), "hh:mm");

    //qDebug() << t1 << t2 << t1.msecsTo(t2);
    bc_timer->setInterval(t1.msecsTo(t2));
    bc_timer->start();
}

// 时间到换班
void Index::bc_timer_timerout()
{
    l_banci = c_banci;
    c_banci = (c_banci+1)%4;
    if(c_banci == 0) // 新的一天
        updateBC();
    //qDebug() << c_banci << "timeout";

    dis_note_last_banci();
    note_curr_banci();
}

void Index::display_bc(QStringList &list)
{
    ui->b1Label->setText(list.at(0));
    ui->b2Label->setText(list.at(4));
    ui->b3Label->setText(list.at(8));
    ui->b4Label->setText(list.at(12));

    /*
     *TODO:
     * 暂停时间
     */
}

// END: 系统设置
//*************************************************************************************************//
