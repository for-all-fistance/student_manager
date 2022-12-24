﻿/******************************************************************************
  * @file    lesson_manage_page.h
  * @author  Jialiang Li
  * @brief   class: lesson_manage_page
  *          info:  created to manage the info of one lesson, including all the students and their info.
  *
*/
#ifndef LESSON_MANAGE_PAGE_H
#define LESSON_MANAGE_PAGE_H

#include <QWidget>

namespace Ui {
class lesson_manage_page;
}

class lesson_manage_page : public QWidget
{
    Q_OBJECT

public:
    explicit lesson_manage_page(QWidget *parent = nullptr);
    ~lesson_manage_page();

private slots:
    void on_turn2student_manage_clicked();
    void do_process_turn2student_manage_signal();

    void on_btn_exit_clicked();

signals:
    void send_turn2student_manage_signal();

private:
    Ui::lesson_manage_page *ui;
    void init();
    void set_content();
};

#endif // LESSON_MANAGE_PAGE_H
