/******************************************************************************
  * @file    add_lesson.cpp
  * @author  Jialiang Li
  * @brief
  *
*/
#pragma execution_character_set("utf-8")

#include "add_lesson.h"
#include "ui_add_lesson.h"
#include <QMessageBox>

add_lesson::add_lesson(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::add_lesson)
{
    ui->setupUi(this);
}

add_lesson::~add_lesson()
{
    delete ui;
}

/**
 * @brief add_lesson::on_confirm_btn_clicked
 */
void add_lesson::on_confirm_btn_clicked()
{
    if (ui->lesson_id->isModified() && ui->lesson_name->isModified() && ui->year->isModified())
    {
        emit send_new_lesson_signal(ui->lesson_name->text(),
            ui->lesson_id->text().toInt(),
            ui->year->text(),
            ui->term->currentText());
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("not modified"),
            QObject::tr("没有完整的输入\n"
                "要新建课程至少需要名称，课程号，学年和学期！\n"), QMessageBox::Cancel);
    }
}

/**
 * @brief add_lesson::on_cancel_btn_clicked
 */
void add_lesson::on_cancel_btn_clicked()
{
    close();
}

