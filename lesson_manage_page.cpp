/******************************************************************************
  * @file    lesson_manage_page.cpp
  * @author  Jialiang Li
  * @brief   member function of class lesson_manage_page, which is created to manage the info of one lesson, including all the students and their info.
  *
*/
#pragma execution_character_set("utf-8")

#include "lesson_manage_page.h"
#include "ui_lesson_manage_page.h"

/**
  * @brief  课程信息管理页面构造函数
  * @param
  * @retval
  */
lesson_manage_page::lesson_manage_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lesson_manage_page)
{
    ui->setupUi(this);
    init();
}

/**
  * @brief  课程信息管理页面析构函数
  * @param
  * @retval
  */
lesson_manage_page::~lesson_manage_page()
{
    delete ui;
}

/**
  * @brief  初始化课程信息界面，并创建login_page和mainwindow之间的逻辑连接
  * @param
  * @retval
  */
void lesson_manage_page::init()
{
    connect(ui->turn2student_manage,SIGNAL(clicked(bool)),this,SLOT(do_process_turn2student_manage_signal()));

    connect(ui->content,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(show_lesson_summery(QTreeWidgetItem*)));
    set_content();//显示列表
}

/**
  * @brief  显示课程信息
  * @param
  * @retval
  */
void lesson_manage_page::set_content()
{
    ui->content->clear();
    ui->content->setSelectionMode(QAbstractItemView::SingleSelection);
    sql_server.read_all_lesson(ui->content);
    ui->content->update();
}

/**
  * @brief  转到学生管理页按钮
  * @param
  * @retval
  */
void lesson_manage_page::on_turn2student_manage_clicked()
{
    this->hide();
    emit send_turn2student_manage_signal();
}

/**
  * @brief  响应从课程管理页面回到学生管理页面的事件，关闭课程信息页面
  * @param
  * @retval
  */
void lesson_manage_page::do_process_turn2student_manage_signal()
{
    this->hide();
}

/**
  * @brief  从课程信息页面退出
  * @param
  * @retval
  */
void lesson_manage_page::on_btn_exit_clicked()

{
    exit(0);
}



/**
 * @brief lesson_manage_page::on_add_score_btn_clicked
 */
void lesson_manage_page::on_add_score_btn_clicked()
{
    sql_server.add_grade();
}


void lesson_manage_page::on_del_score_btn_clicked()
{
    //sql_server.delete_grade();
}


void lesson_manage_page::on_find_stu_clicked()
{
    if(ui->search_bar_lesson->isModified())
    {
        ui->content->clear();
        sql_server.search_for_student(ui->search_bar_lesson->text(),ui->content);
        ui->content->update();
    }
    else
    {
        refresh();
    }
}

/**
 * @brief lesson_manage_page::show_lesson_info 当选中不同的课程时显示相应的课程概述
 * @param my_lesson
 */
void lesson_manage_page::show_lesson_summery(QTreeWidgetItem* my_lesson)
{
    QString total_cnt=sql_server.get("lesson_info","total_count","lesson_id",my_lesson->text(1));
    QString avrg_score=sql_server.get("lesson_info","average_score","lesson_id",my_lesson->text(1));
    QString pass_rt=sql_server.get("lesson_info","pass_rate","lesson_id",my_lesson->text(1));
    ui->total_stu->setText("总人数："+total_cnt);
    ui->pass_rate->setText("及格率："+pass_rt);
    ui->average->setText("平均分："+avrg_score);

}

void lesson_manage_page::on_add_lesson_btn_clicked()
{

}

/**
 * @brief lesson_manage_page::refresh 刷新
 */
void lesson_manage_page::refresh()
{
    sql_server.clac_lesson_summery();
    set_content();
}

