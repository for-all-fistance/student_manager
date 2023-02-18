/******************************************************************************
  * @file    lesson_manage_page.h
  * @author  Jialiang Li
  * @brief   class: lesson_manage_page
  *          info:  created to manage the info of one lesson, including all the students and their info.
  *
*/
#ifndef LESSON_MANAGE_PAGE_H
#define LESSON_MANAGE_PAGE_H

#include <QWidget>
#include "file_browser.h"
#include "global.h"
#include "ssql.h"

namespace Ui
{
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
    void on_add_score_btn_clicked();
    void on_find_lesson_clicked();
    void show_lesson_summery(QTreeWidgetItem *my_lesson);
    void on_add_lesson_btn_clicked();
    void on_del_btn_clicked();
    void refresh();
    void qtreewidget_open_editor(QTreeWidgetItem *item, int col);
    void qtreewidget_close_editor(QTreeWidgetItem *item, int col);
    void on_read_file_btn_clicked();
signals:
    void send_turn2student_manage_signal();

private:
    Ui::lesson_manage_page *ui;
    void init();
    void set_content();
    file_browser *my_browser;
    sSql sql_server;
};

#endif // LESSON_MANAGE_PAGE_H
