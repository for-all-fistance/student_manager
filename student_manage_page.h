/******************************************************************************
  * @file    mainwindow.h
  * @author  Jialiang Li
  * @brief   class: MainWindow
  *          info:  主页面，也充当学生信息管理的页面
  *
*/
#ifndef STUDENT_MANAGE_PAGE_H
#define STUDENT_MANAGE_PAGE_H

#include "login_page.h"
#include "lesson_manage_page.h"
#include "ssql.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class student_manage_page : public QMainWindow
{
    Q_OBJECT

public:
    student_manage_page(QWidget *parent = nullptr);
    ~student_manage_page();

private slots:
    void on_btn_exit_clicked();
    void on_btn_relogin_clicked();
    void set_content();
    void on_turn2lesson_manage_clicked();
    void do_process_turn_back_request();
    void do_process_login_request();
    void on_add_stu_btn_clicked();
    void on_find_stu_btn_clicked();
    void qtreewidget_open_editor(QTreeWidgetItem *item, int col);
    void qtreewidget_close_editor(QTreeWidgetItem *item, int col);
    void refresh();
    void on_del_btn_clicked();
    void on_add_grade_btn_clicked();
    void on_read_file_btn_clicked();

private:
    Ui::MainWindow *ui;
    void init();
    login_page *login;
    lesson_manage_page *lesson_manage;
    file_browser *my_browser;
    sSql *sql_server;
};
#endif // STUDENT_MANAGE_PAGE_H
