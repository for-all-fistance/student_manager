#include "add_student.h"
#include "ui_add_student.h"

add_student::add_student(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_student)
{
    ui->setupUi(this);

}

add_student::~add_student()
{
    delete ui;
}

/**
 * @brief add_student::on_new_stud_clicked
 */
void add_student::on_new_stud_clicked()
{
    this->hide();
}


