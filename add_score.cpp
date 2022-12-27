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

void add_score::on_new_score_btn_clicked()
{
    this->hide();
}

