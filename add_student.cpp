/******************************************************************************
  * @file    add_student.cpp
  * @author  Jialiang Li
  * @brief
  *
*/
#pragma execution_character_set("utf-8")

#include "add_student.h"
#include "ui_add_student.h"
#include <QMessageBox>

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
 * @brief add_student::on_new_stud_btn_clicked
 */
void add_student::on_new_stud_btn_clicked()
{
    if (ui->class_->isModified() && ui->stud_id->isModified() && ui->stud_name->isModified()) //如果有输入，那就发送信号
    {
        emit send_new_student_signal(ui->stud_id->text().toInt(), ui->stud_name->text(), ui->class_->text());
        close();
    }
    else//没有就弹出提示
    {
        QMessageBox::critical(this, QObject::tr("not modified"),
            QObject::tr("没有完整的输入\n"
                "要新建学生至少需要学号，姓名和班级！\n"), QMessageBox::Cancel);
    }
}

/**
 * @brief add_student::on_cancel_btn_clicked
 */
void add_student::on_cancel_btn_clicked()
{
    close();
}

