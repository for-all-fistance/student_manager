#pragma execution_character_set("utf-8")

#include "add_score.h"
#include "ui_add_score.h"

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

QString add_score::get_lesson_name()
{
    return ui->lesson_name->text();
}

QString add_score::get_stud_name()
{
    return ui->stud_name->text();
}

float add_score::get_score()
{
    return ui->score->text().toFloat();
}

QString add_score::get_term()
{
    return ui->term->text();
}

QString add_score::get_year()
{
    return ui->year->text();
}

/**
 * @brief add_score::on_new_score_btn_clicked 槽函数，向sql模块发送信号，请求增加新grade对象
 */
void add_score::on_new_score_btn_clicked()
{
    emit send_new_score_signal(ui->stud_name->text(),ui->lesson_name->text(),ui->year->text(),ui->term->text(),ui->score->text().toFloat());
    close();
}

/**
 * @brief add_score::on_cancel_clicked 关闭界面
 */
void add_score::on_cancel_clicked()
{
    close();
}

