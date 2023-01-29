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
#include "global.h"

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

void MainWindow::do_process_login_request(QString user_name)
/**
  * @brief  响应登录事件
  * @param
  * @retval
  */
{

    login.close();
    ui->username_main->setText(user_name);
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
    ui->content->setSelectionMode(QAbstractItemView::MultiSelection);
    sql_server.read_all_student(ui->content);
    ui->content->update();
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
    refresh();
    show();
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
void MainWindow::on_del_btn_clicked()
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
void MainWindow::on_find_stu_btn_clicked()
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
void MainWindow::qtreewidget_open_editor(QTreeWidgetItem *item, int col)
{
    if(col!=1)//不能修改学号
        ui->content->openPersistentEditor(item,col);
}

/**
 * @brief MainWindow::qtreewidget_close_editor 结束编辑并保存更改
 */
void MainWindow::qtreewidget_close_editor(QTreeWidgetItem *item,int col)
{
    if(item!=NULL)
    {
       ui->content->closePersistentEditor(item,col);
       switch (item->type()) {
       case STUDENT:
           sql_server.update(item->text(0),item->text(1).toInt(),item->text(2),item->text(3));
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
void MainWindow::refresh()
{
    sql_server.calc_performance();
    set_content();
}



void MainWindow::on_add_grade_btn_clicked()
{
    sql_server.add_grade();
}

