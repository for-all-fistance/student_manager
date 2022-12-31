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
    set_content();
}

/**
  * @brief  显示课程信息
  * @param
  * @retval
  */
void lesson_manage_page::set_content()
{
    int row=4,col=4,student_count=20;
    //设置行，列数
     ui->content->setColumnCount(row);
     ui->content->setColumnCount(col);
    //调整大小和位置
    //ui->content->move(ui->topic->pos());
    //设置表格内容
     QStringList header({"姓名","学号","班级","成绩"});
     ui->content->setHeaderLabels(header);
     for(int i=0;i<student_count;i++)
     {
         QTreeWidgetItem* new_top_item = new QTreeWidgetItem(QStringList({"张三","1000","3","60"}));
         ui->content->addTopLevelItem(new_top_item);
     }
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
    sql_server.delete_grade();
}

