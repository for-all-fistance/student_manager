/******************************************************************************
  * @file    mainwindow.cpp
  * @author  Jialiang Li
  * @brief   member function of class MainWindow,including the main window UI and
  *
*/
#pragma execution_character_set("utf-8")

#include "student_manage_page.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QTableWidget>
#include "global.h"

student_manage_page::student_manage_page(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

student_manage_page::~student_manage_page()
{
    delete ui;
}

/**
 * @brief MainWindow::init 初始化登陆界面，并创建login_page，lesson_manage_page槽之间的逻辑连接
 */
void student_manage_page::init()
{
    login.show();//显示登录界面
    set_content();
    connect(&login,SIGNAL(send_login_success_signal(QString)),this,SLOT(do_process_login_request(QString)));//收到登陆成功信号,打开主界面
    connect(&login,SIGNAL(send_login_success_signal(QString)),&lesson_manage,SLOT(do_process_login_request(QString)));
    connect(&lesson_manage,SIGNAL(send_turn2student_manage_signal()),this,SLOT(do_process_turn_back_request()));
    connect(&sql_server,SIGNAL(send_student_added_signal()),this,SLOT(refresh()));//当数据发生变化时，刷新页面
    connect(&sql_server,SIGNAL(send_grade_added_signal()),this,SLOT(refresh()));
    connect(&sql_server,SIGNAL(send_lesson_added_signal()),this,SLOT(refresh()));
    connect(ui->content,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(qtreewidget_open_editor(QTreeWidgetItem*,int)));//双击编辑
    connect(ui->content,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(qtreewidget_close_editor(QTreeWidgetItem*,int)));//确认编辑

}

/**
 * @brief MainWindow::do_process_login_request 响应登录信号
 * @param user_name
 */
void student_manage_page::do_process_login_request(QString user_name)
{

    login.close();
    ui->username_main->setText(user_name);
    this->show();
}

void student_manage_page::on_btn_exit_clicked()
{
    exit(0);
}

/**
 * @brief MainWindow::on_btn_relogin_clicked 重新登录按钮，退出主界面，打开登录界面
 */
void student_manage_page::on_btn_relogin_clicked()
{
    login.show();
    this->hide();
}

/**
 * @brief MainWindow::set_content 整理QTreeWidget窗口
 */
void student_manage_page::set_content()
{
    ui->content->clear();
    ui->content->setSelectionMode(QAbstractItemView::MultiSelection);
    sql_server.read_all_student(ui->content);
    ui->content->update();
}


void student_manage_page::on_turn2lesson_manage_clicked()
{
    this->hide();
    lesson_manage.show();
}

/**
 * @brief MainWindow::do_process_turn_back_request 响应回到学生管理页面的事件，显示学生管理页面
 */
void student_manage_page::do_process_turn_back_request()
{
    refresh();
    show();
}

/**
 * @brief MainWindow::on_add_stu_clicked
 */
void student_manage_page::on_add_stu_btn_clicked()
{
    sql_server.add_student();
}

/**
 * @brief MainWindow::on_del_stu_clicked 删除一个学生
 */
void student_manage_page::on_del_btn_clicked()
{
    QList<QTreeWidgetItem*> selectedItems=ui->content->selectedItems();
    foreach (QTreeWidgetItem *item, selectedItems) {
        switch (item->type()) {
        case STUDENT:
            sql_server.delete_student(item->text(1).toInt());
            break;
        case GRADE:
            sql_server.delete_grade(item->parent()->text(0),item->text(0),item->text(2),item->text(3));
            break;
        default:
            break;
        }
    }
    refresh();
}

/**
 * @brief MainWindow::on_find_stu_clicked 查找学生
 */
void student_manage_page::on_find_stu_btn_clicked()
{
    if(ui->search_bar_stu->isModified())//有输入
    {
        ui->content->clear();
        sql_server.search_for_student(ui->search_bar_stu->text(),ui->content);
        ui->content->update();
    }
    else
        refresh();
}

/**
 * @brief MainWindow::qtreewidget_open_editor 双击编辑
 * @param item 被编辑的项
 * @param col 该项所在的列
 */
void student_manage_page::qtreewidget_open_editor(QTreeWidgetItem *item, int col)
{
    if(col!=1&&col!=3)//不能修改学号和优良等级，前者只能在创建时指定，后者由系统自动计算
        ui->content->openPersistentEditor(item,col);
}

/**
 * @brief MainWindow::qtreewidget_close_editor 结束编辑并保存更改
 */
void student_manage_page::qtreewidget_close_editor(QTreeWidgetItem *item,int col)
{
    if(item!=NULL)
    {
       ui->content->closePersistentEditor(item,col);
       switch (item->type()) {
       case STUDENT:
           sql_server.update(item->text(0),item->text(1).toInt(),item->text(2));
           break;
       case GRADE:
           sql_server.update(item->parent()->text(0),item->text(0),item->text(1).toFloat());
           break;
       default:
           break;
       }
    }
}

/**
 * @brief MainWindow::refresh 刷新
 */
void student_manage_page::refresh()
{
    sql_server.calc_performance();
    set_content();
}


void student_manage_page::on_add_grade_btn_clicked()
{
    sql_server.add_grade();
}

/**
 * @brief MainWindow::on_read_file_btn_clicked 唤起一个file browser页面来读取文件
 */
void student_manage_page::on_read_file_btn_clicked()
{
    my_browser=new file_browser();
    connect(my_browser,SIGNAL(send_file_read_signal()),this,SLOT(refresh()));
}

