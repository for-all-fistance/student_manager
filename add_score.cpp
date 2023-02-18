#pragma execution_character_set("utf-8")

#include "add_score.h"
#include "ui_add_score.h"
#include <QMessageBox>

add_score::add_score(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_score)
{
    ui->setupUi(this);
}

add_score::~add_score()
{
    delete ui;
}

/**
 * @brief add_score::on_new_score_btn_clicked 槽函数，向sql模块发送信号，请求增加新grade对象
 */
void add_score::on_new_score_btn_clicked()
{
    if (ui->lesson_name->isModified() && ui->stud_name->isModified() && ui->year->isModified() && ui->score->isModified())
    {
        emit send_new_score_signal(ui->stud_name->text(), ui->lesson_name->text(), ui->year->text(), ui->term->currentText(), ui->score->text().toFloat());
        close();
    }
    else
        QMessageBox::critical(this, QObject::tr("not modfied"),
            QObject::tr("没有完整的输入，"
                "请输入姓名，课程，学年，学期和成绩来创建一个成绩！"), QMessageBox::Cancel);
}

/**
 * @brief add_score::on_cancel_clicked 关闭界面
 */
void add_score::on_cancel_clicked()
{
    close();
}

