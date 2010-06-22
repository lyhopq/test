#include "kbdialog.h"
#include "ui_kbdialog.h"

#include <QMessageBox>
#include <QKeyEvent>
//#include <QDebug>

KBDialog::KBDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KBDialog)
{
    ui->setupUi(this);
    this->setFixedSize(500,250);

    //this->setWindowFlags(Qt::CustomizeWindowHint);

    //this->installEventFilter(this);

    QList<QPushButton *> allPButtons = ui->keyFrame->findChildren<QPushButton *>();
    //qDebug() << allPButtons.size();
    for(int i = 0; i < allPButtons.size(); i++)
    {
        allPButtons[i]->installEventFilter(this);
    }

    ui->pb_per->setDisabled(true);
    ui->pb_next->setDisabled(true);

    //this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_InputMethodEnabled);
    //this->setFocusPolicy(Qt::NoFocus);
    //this->setFocus();

    //this->setStyleSheet();

    isShiftDown = false;
    findNothing = true;
    pageCount = 0;
    currentPage = 0;
    //currentZh = 0;

    file.setFileName("py.txt");
    out.setFileName("serch.txt");
    if(!out.open(QIODevice::ReadWrite | QIODevice::Truncate))
        QMessageBox::warning(0, tr("初始化错误"),
            tr("无法写入查询数据，请检查文件是否有效!"));
    if(!file.open(QIODevice::ReadOnly))
        QMessageBox::warning(0, tr("加载错误"),
            tr("无法加载字符编码，请检查文件是否有效！"));

    regExp.setPatternSyntax(QRegExp::RegExp);
    regExp.setCaseSensitivity(Qt::CaseSensitive);

    changKeyboard();
}

KBDialog::~KBDialog()
{
    delete ui;
}

void KBDialog::changeEvent(QEvent *e)
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


void KBDialog::changKeyboard()
{
    if(!isShiftDown)
    {
        ui->pb_esc->setText("Esc");
        ui->pb_com->setText("`");
        ui->pb_1->setText("1");
        ui->pb_2->setText("2");
        ui->pb_3->setText("3");
        ui->pb_4->setText("4");
        ui->pb_5->setText("5");
        ui->pb_6->setText("6");
        ui->pb_7->setText("7");
        ui->pb_8->setText("8");
        ui->pb_9->setText("9");
        ui->pb_0->setText("0");
        ui->pb_mi->setText("-");
        ui->pb_equ->setText("=");

        ui->pb_tab->setText("Tab");
        ui->pb_q->setText("q");
        ui->pb_w->setText("w");
        ui->pb_e->setText("e");
        ui->pb_r->setText("r");
        ui->pb_t->setText("t");
        ui->pb_y->setText("y");
        ui->pb_u->setText("u");
        ui->pb_i->setText("i");
        ui->pb_o->setText("o");
        ui->pb_p->setText("p");
        ui->pb_backslash->setText("\\");
        //ui->pb_backspace->setText("BackSpace");

        ui->pb_shift->setText("Shift");
        ui->pb_a->setText("a");
        ui->pb_s->setText("s");
        ui->pb_d->setText("d");
        ui->pb_f->setText("f");
        ui->pb_g->setText("f");
        ui->pb_h->setText("h");
        ui->pb_j->setText("j");
        ui->pb_k->setText("k");
        ui->pb_l->setText("l");
        ui->pb_colon->setText(";");
        ui->pb_enter->setText("Enter");

        ui->pb_change->setText(tr("中"));
        ui->pb_z->setText("z");
        ui->pb_x->setText("x");
        ui->pb_c->setText("c");
        ui->pb_v->setText("v");
        ui->pb_b->setText("b");
        ui->pb_n->setText("n");
        ui->pb_m->setText("m");
        ui->pb_comma->setText(",");
        //ui->pb_up->setText("up");
        ui->pb_period->setText(".");
        ui->pb_semicolon->setText("'");
        ui->pb_slash->setText("/");

        ui->pb_ctrl->setText("Ctrl");
        ui->pb_win->setText("Win");
        ui->pb_alt->setText("Alt");
        ui->pb_space->setText("Space");
        ui->pb_zuofang->setText("[");
        ui->pb_youfang->setText("]");
        //ui->pb_left->setText("left");
        //ui->pb_down->setText("down");
        //ui->pb_right->setText("right");
        ui->pb_func->setText("func");
    }
    else
    {
        //ui->pb_esc->setText("Esc");
        ui->pb_com->setText("~");
        ui->pb_1->setText("!");
        ui->pb_2->setText("@");
        ui->pb_3->setText("#");
        ui->pb_4->setText("$");
        ui->pb_5->setText("%");
        ui->pb_6->setText("^");
        ui->pb_7->setText("&&");
        ui->pb_8->setText("*");
        ui->pb_9->setText("(");
        ui->pb_0->setText(")");
        ui->pb_mi->setText("_");
        ui->pb_equ->setText("+");

        //ui->pb_tab->setText("Tab");
        ui->pb_q->setText("Q");
        ui->pb_w->setText("W");
        ui->pb_e->setText("E");
        ui->pb_r->setText("R");
        ui->pb_t->setText("T");
        ui->pb_y->setText("Y");
        ui->pb_u->setText("U");
        ui->pb_i->setText("I");
        ui->pb_o->setText("O");
        ui->pb_p->setText("P");
        ui->pb_backslash->setText("|");
        //ui->pb_backspace->setText("BackSpace");

        //ui->pb_shift->setText("Shift");
        ui->pb_a->setText("A");
        ui->pb_s->setText("S");
        ui->pb_d->setText("D");
        ui->pb_f->setText("F");
        ui->pb_g->setText("G");
        ui->pb_h->setText("H");
        ui->pb_j->setText("J");
        ui->pb_k->setText("K");
        ui->pb_l->setText("L");
        ui->pb_colon->setText(":");
        //ui->pb_enter->setText("Enter");

        //ui->pb_change->setText(tr("中/英"));
        ui->pb_z->setText("Z");
        ui->pb_x->setText("X");
        ui->pb_c->setText("C");
        ui->pb_v->setText("V");
        ui->pb_b->setText("B");
        ui->pb_n->setText("N");
        ui->pb_m->setText("M");
        ui->pb_comma->setText("<");
        //ui->pb_up->setText("up");
        ui->pb_period->setText(">");
        ui->pb_semicolon->setText("\"");
        ui->pb_slash->setText("?");

        //ui->pb_ctrl->setText("Ctrl");
        //ui->pb_win->setText("Win");
        //ui->pb_alt->setText("Alt");
        //ui->pb_space->setText("Space");
        ui->pb_zuofang->setText("{");
        ui->pb_youfang->setText("}");
        //ui->pb_left->setText("left");
        //ui->pb_down->setText("down");
        //ui->pb_right->setText("right");
        //ui->pb_func->setText("func");

    }
}

void KBDialog::on_pb_change_clicked()
{
    if(method == english)
    {
        method = chinese;
        ui->pb_change->setText(tr("中"));
    }
    else
    {
        method = english;
        ui->pb_change->setText(tr("英"));
    }
}

void KBDialog::on_pb_shift_clicked()
{
    isShiftDown = !isShiftDown;

    changKeyboard();
}

QString KBDialog::transPy(QString pyInput)
{
    py.append(pyInput);
    ui->pyLineEdit->insert(pyInput);

    out.open(QIODevice::ReadWrite | QIODevice::Truncate);
    int ret = 0;
    int count = 0;    // 统计匹配的字符数量

    regExp.setPattern(QString("([^a-z\\s]{2,8})(%1)(\\s)").arg(py));

    while(!file.atEnd())
    {
        QByteArray data = file.readLine();
        ret = regExp.indexIn(QString(data.data()), 0, QRegExp::CaretAtZero);

        if(ret > -1)     // 如果匹配
        {
            findNothing = false;
            //zhlist << regExp.cap(1).toAscii().data();
            //zhlist << regExp.cap(1).toAscii().data();
            out.write(regExp.cap(1).toAscii().data());
            out.write("\n");
            count++;

        }
        //qDebug() << zhlist;
    }
    file.seek(0);
    out.close();

    if(count%8 == 0)
    {
        pageCount = count/8;
    }
    else
    {
        pageCount = (count/8)+1;
    }
    if(pageCount > 0)
        ui->pb_next->setEnabled(true);


    if(findNothing)    // 没有匹配字符
    {
        this->clearZh();
        return NULL;
    }

    // 如果找到匹配
    findNothing = true;
    showPage(0);     // 显示第一页

    return py;
}

void KBDialog::showPage(int index)
{
    int n = 0;
    QString chinese[8];

    out.open(QIODevice::ReadWrite);

    //while(currentZh < zhlist.size())
    //if(currentZh < zhlist.size())
    while(!out.atEnd())
    {
        for(int j = 0; j < 8; j++)
        {
            //currentZh++;
            QByteArray data = out.readLine();
            data.replace("\n","\0");
            n++;
            //if((index*8) < currentZh && currentZh < ((index+1)*8+1))
            if((index*8) < n && n < ((index+1)*8+1))
            {
                //chinese[j].append(zhlist.at(currentZh-1));
                chinese[j].append(data);
            }
        }
    }

    //qDebug() << chinese[0] << chinese[1] << chinese[2] << chinese[3]
    //    << chinese[4] << chinese[5] << chinese[5] << chinese[6] << chinese[7];

    ui->pb_h1->setText(tr(chinese[0].toAscii()));
    ui->pb_h2->setText(tr(chinese[1].toAscii()));
    ui->pb_h3->setText(tr(chinese[2].toAscii()));
    ui->pb_h4->setText(tr(chinese[3].toAscii()));
    ui->pb_h5->setText(tr(chinese[4].toAscii()));
    ui->pb_h6->setText(tr(chinese[5].toAscii()));
    ui->pb_h7->setText(tr(chinese[6].toAscii()));
    ui->pb_h8->setText(tr(chinese[7].toAscii()));

    out.close();

}

void KBDialog::on_pb_per_clicked()
{
    currentPage--;
    if(currentPage > -1)
    {
        ui->pb_next->setEnabled(true);
        if(currentPage-- == 0)
        {
            ui->pb_per->setDisabled(true);
        }
        currentPage++;
        showPage(currentPage);
    }
    else
    {
        currentPage++;
        ui->pb_per->setDisabled(true);
    }
}

void KBDialog::on_pb_next_clicked()
{
    currentPage++;
    if(currentPage < pageCount)
    {
        ui->pb_per->setEnabled(true);
        if(currentPage++ == pageCount-1)
            ui->pb_next->setDisabled(true);

        currentPage--;
        showPage(currentPage);
    }
    else
    {
        currentPage--;
        ui->pb_next->setDisabled(true);
    }
}

void KBDialog::clearZh()
{
    ui->pb_h1->setText("");
    ui->pb_h2->setText("");
    ui->pb_h3->setText("");
    ui->pb_h4->setText("");
    ui->pb_h5->setText("");
    ui->pb_h6->setText("");
    ui->pb_h7->setText("");
    ui->pb_h8->setText("");

    //zhlist.clear();
    currentPage = 0;

    ui->pb_per->setDisabled(true);
    ui->pb_next->setDisabled(true);
}

void KBDialog::on_pb_h1_clicked()
{
    ui->rlineEdit->insert(ui->pb_h1->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}

void KBDialog::on_pb_h2_clicked()
{
    ui->rlineEdit->insert(ui->pb_h2->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}

void KBDialog::on_pb_h3_clicked()
{
    ui->rlineEdit->insert(ui->pb_h3->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}
void KBDialog::on_pb_h4_clicked()
{
    ui->rlineEdit->insert(ui->pb_h4->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}
void KBDialog::on_pb_h5_clicked()
{
    ui->rlineEdit->insert(ui->pb_h5->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}
void KBDialog::on_pb_h6_clicked()
{
    ui->rlineEdit->insert(ui->pb_h6->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}
void KBDialog::on_pb_h7_clicked()
{
    ui->rlineEdit->insert(ui->pb_h7->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}
void KBDialog::on_pb_h8_clicked()
{
    ui->rlineEdit->insert(ui->pb_h8->text());
    py.clear();
    ui->pyLineEdit->clear();

    this->clearZh();
}

QKeyEvent* KBDialog::transKey(QPushButton *obj)
{
    QKeyEvent *key;

    if(obj == ui->pb_backspace)
    {
        if(method == english)
            key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        else
            key = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier,"BackSpace");
    }
    else if(obj == ui->pb_enter)
    {
        key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
    }
    else if(obj == ui->pb_esc)
    {
        key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
    }
    else if(obj == ui->pb_tab)
    {
        key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
    }
    else if(obj == ui->pb_shift || obj == ui->pb_change || obj == ui->pb_func
         || obj == ui->pb_ctrl || obj == ui->pb_win || obj == ui->pb_alt || obj == ui->pb_space
         || obj == ui->pb_left || obj == ui->pb_right || obj == ui->pb_up || obj == ui->pb_down)
    {
        //key = new QKeyEvent(QEvent::KeyPress, Qt::Key_Shift, Qt::NoModifier);
        return NULL;
    }

    else
    {
        QString tmp = obj->text();
        key = new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier, tmp);
    }

    return key;
}

bool KBDialog::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *key;
    //qDebug() << "i am here!" << event->type();

    if(event->type() == QEvent::MouseButtonPress)
    {
        //QMouseEvent *mouse = (QMouseEvent *)event;

        if((key = transKey((QPushButton *)obj)) != NULL)
        {
            if(method == english)     // 英文输入法
            {
                QCoreApplication::postEvent(QApplication::focusWidget(), key);
                return true;
            }
            else    // 中文输入法
            {
                QString c = key->text();

                if(c == "BackSpace")
                {
                    ui->pyLineEdit->backspace();
                    py.clear();

                    c = ui->pyLineEdit->text();
                    ui->pyLineEdit->clear();
                }

                //qDebug() << key->text();
                if(c == "")
                    this->clearZh();
                else if( transPy(c) != NULL)
                {
                    return true;
                }

                return false;
            }
        }
    }
    return false;

}

void KBDialog::on_pb_ok_clicked()
{
    res = ui->rlineEdit->text();
    ui->rlineEdit->clear();

    accept();
}

void KBDialog::on_pb_cancel_clicked()
{
    ui->rlineEdit->clear();
    this->clearZh();

    reject();
}
