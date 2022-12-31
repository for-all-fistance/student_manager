/******************************************************************************
  * @file    mainwindow.cpp
  * @author  Jialiang Li
  * @brief   member function of class MainWindow,including the main window UI and
  *
*/
#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
/**
  * @brief  主界面构造函数
  * @param
  * @retval
  */
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
/**
  * @brief  主界面析构函数
  * @param
  * @retval
  */
{
    delete ui;
}

void MainWindow::init()
/**
  * @brief  初始化登陆界面，并创建login_page，lesson_manage_page槽之间的逻辑连接
  * @param
  * @retval
  */
{
    login.show();//显示登录界面
    connect(&login,SIGNAL(send_login_success_signal()),this,SLOT(do_process_login_request()));//收到登陆成功信号,打开主界面
    connect(&lesson_manage,SIGNAL(send_turn2student_manage_signal()),this,SLOT(do_process_turn_back_request()));//如果点击返回学生信息页面的按钮，将打开主页面（学生信息页面）
    connect(&sql_server,SIGNAL(send_student_deleted_signal()),this,SLOT(ui->content->update()));//当数据发生变化时，刷新页面
    connect(&sql_server,SIGNAL(send_student_added_signal()),this,SLOT(ui->content->update()));
    connect(&sql_server,SIGNAL(send_grade_added_signal()),this,SLOT(ui->content->update()));
    connect(&sql_server,SIGNAL(send_grade_deleted_signal()),this,SLOT(ui->content->update()));
    connect(&sql_server,SIGNAL(send_lesson_deleted_signal()),this,SLOT(ui->content->update()));
    connect(&sql_server,SIGNAL(send_lesson_added_signal()),this,SLOT(ui->content->update()));
    set_content();
}

void MainWindow::do_process_login_request()
/**
  * @brief  响应登录事件
  * @param
  * @retval
  */
{

    login.hide();
    this->show();
}

void MainWindow::on_btn_exit_clicked()
/**
  * @brief  主界面退出按钮
  * @param
  * @retval
  */
{
    exit(0);
}


void MainWindow::on_btn_relogin_clicked()
/**
  * @brief  重新登录按钮，退出主界面，打开登录界面
  * @param
  * @retval
  */
{
    login.show();
    this->hide();
}

void MainWindow::set_content()
/**
  * @brief  设置主界面信息
  * @param
  * @retval
  */
{
    ui->content->clear();
    sql_server.read_all_student(ui->content);
}


void MainWindow::on_turn2lesson_manage_clicked()
/**
  * @brief  转到课程管理界面
  * @param
  * @retval
  */
{
    this->hide();
    lesson_manage.show();
}

void MainWindow::do_process_turn_back_request()
/**
  * @brief  响应回到学生管理页面的事件，显示学生管理页面
  * @param
  * @retval
  */
{
    this->show();
}

/**
 * @brief MainWindow::on_add_stu_clicked
 */
void MainWindow::on_add_stu_btn_clicked()
{
    sql_server.add_student();
}

/**
 * @brief MainWindow::on_del_stu_clicked 删除一个学生
 */
void MainWindow::on_del_stu_btn_clicked()
{
    sql_server.delete_student();
}

/**
 * @brief MainWindow::on_find_stu_clicked 查找学生
 */
void MainWindow::on_find_stu_btn_clicked()
{
    if(ui->search_bar_stu->isModified())//有输入
    {
        sql_server.search_for_student(ui->search_bar_stu->text(),ui->content);
    }
    else//无输入，刷新界面
        set_content();
}

