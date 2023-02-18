/******************************************************************************
  * @file    lesson_manage_page.cpp
  * @author  Jialiang Li
  * @brief   member function of class lesson_manage_page, which is created to manage the info of each lesson, including all the students and their info.
  *
*/
#pragma execution_character_set("utf-8")

#include "lesson_manage_page.h"
#include "ui_lesson_manage_page.h"
#include "login_page.h"

lesson_manage_page::lesson_manage_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lesson_manage_page)
{
    ui->setupUi(this);
    init();
}

lesson_manage_page::~lesson_manage_page()
{
    delete ui;
}

/**
 * @brief lesson_manage_page::init 初始化课程信息界面，并创建login_page和mainwindow之间的逻辑连接
 */
void lesson_manage_page::init()
{
    isediting = false;
    set_content();//显示列表
    connect(&sql_server, SIGNAL(send_student_added_signal()), this, SLOT(refresh())); //当数据发生变化时，刷新页面
    connect(&sql_server, SIGNAL(send_grade_added_signal()), this, SLOT(refresh()));
    connect(&sql_server, SIGNAL(send_lesson_added_signal()), this, SLOT(refresh()));
    connect(ui->content, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
        this, SLOT(qtreewidget_open_editor(QTreeWidgetItem *, int))); //双击编辑
    connect(ui->content, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(qtreewidget_close_editor(QTreeWidgetItem *, int))); //确认编辑
    connect(ui->content, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(show_lesson_summery(QTreeWidgetItem *)));
}

/**
 * @brief lesson_manage_page::set_content 整理QTreeWidget窗口
 */
void lesson_manage_page::set_content()
{
    ui->content->clear();
    ui->content->setSelectionMode(QAbstractItemView::SingleSelection);
    sql_server.read_all_lesson(ui->content);
    ui->username_lesson->setText(CURRENT_USER);
    ui->content->update();
}

void lesson_manage_page::on_turn2student_manage_clicked()
{
    this->close();
    emit send_turn2student_manage_signal();
}

/**
 * @brief lesson_manage_page::do_process_turn2student_manage_signal 响应从课程管理页面回到学生管理页面的事件，关闭课程信息页面
 */
void lesson_manage_page::do_process_turn2student_manage_signal()
{
    this->close();
}

/**
 * @brief lesson_manage_page::on_btn_exit_clicked 从课程信息页面退出，关闭程序
 */
void lesson_manage_page::on_btn_exit_clicked()

{
    exit(0);
}


void lesson_manage_page::on_add_score_btn_clicked()
{
    sql_server.add_grade();
}

/**
 * @brief lesson_manage_page::on_find_lesson_clicked 点击搜索按钮，如果搜索框内有内容，就搜索
 */
void lesson_manage_page::on_find_lesson_clicked()
{
    QString input = ui->search_bar_lesson->text();
    if (input.isEmpty()) //无输入
    {
        refresh();
    }
    else//有输入
    {
        ui->content->clear();
        sql_server.search_for_lesson(input, ui->content);
        ui->content->update();
    }
}

/**
 * @brief lesson_manage_page::show_lesson_info 当选中不同的课程时显示相应的课程概述
 * @param my_lesson
 */
void lesson_manage_page::show_lesson_summery(QTreeWidgetItem *my_lesson)
{
    my_lesson = my_lesson->type() == LESSON ? my_lesson : my_lesson->parent();
    QString total_cnt = sql_server.get("lesson_info", "total_count", "lesson_id", my_lesson->text(1));
    QString avrg_score = sql_server.get("lesson_info", "average_score", "lesson_id", my_lesson->text(1)); //保留四位有效数字
    QString pass_rt = sql_server.get("lesson_info", "pass_rate", "lesson_id", my_lesson->text(1));
    QString perfect = sql_server.get("lesson_info", "perfect", "lesson_id", my_lesson->text(1));
    QString good = sql_server.get("lesson_info", "good", "lesson_id", my_lesson->text(1));
    QString fail = sql_server.get("lesson_info", "fail", "lesson_id", my_lesson->text(1));
    QString qualifeid = sql_server.get("lesson_info", "qualified", "lesson_id", my_lesson->text(1));
    ui->total_stu->setText("总人数：" + total_cnt);
    ui->pass_rate->setText("及格率：" + pass_rt.mid(0, 4) + "%");
    ui->average->setText("平均分：" + avrg_score.mid(0, 4));
    ui->perfect->setText("优秀人数：" + perfect);
    ui->good->setText("良好人数：" + good);
    ui->qualified->setText("合格人数：" + qualifeid);
    ui->fail->setText("不及格人数：" + fail);
}

void lesson_manage_page::on_add_lesson_btn_clicked()
{
    sql_server.add_lesson();
}

/**
 * @brief lesson_manage_page::refresh 刷新
 */
void lesson_manage_page::refresh()
{
    sql_server.clac_lesson_summery();
    set_content();
}

/**
 * @brief lesson_manage_page::on_del_btn_clicked 删除选中的项
 */
void lesson_manage_page::on_del_btn_clicked()
{
    QList<QTreeWidgetItem *> selectedItems = ui->content->selectedItems();
    foreach (QTreeWidgetItem *item, selectedItems)
    {
        switch (item->type())
        {
            case LESSON:
                sql_server.delete_lesson(item->text(1).toInt());
                break;
            case GRADE:
                sql_server.delete_grade(item->text(1).toInt(), item->parent()->text(1).toInt());
                break;
            default:
                break;
        }
    }
    refresh();
}

/**
 * @brief lesson_manage_page::set_username 在窗口右上角显示用户名
 * @param user_name
 */


/**
 * @brief lesson_manage_page::qtreewidget_open_editor 双击编辑
 * @param item
 * @param col
 */
void lesson_manage_page::qtreewidget_open_editor(QTreeWidgetItem *item, int col)
{
    ui->content->openPersistentEditor(item, col);
    isediting = true;
}

/**
 * @brief lesson_manage_page::qtreewidget_close_editor 退出编辑并更新数据
 * @param item
 * @param col
 */
void lesson_manage_page::qtreewidget_close_editor(QTreeWidgetItem *item, int col)
{
    if (item != NULL && isediting)
    {
        ui->content->closePersistentEditor(item, col);
        switch (item->type())
        {
            case LESSON:
                if (col != 1) //不能修改课程号
                {
                    sql_server.update(item->text(1).toInt(), item->text(0), item->text(2), item->text(3));
                }
                break;
            case GRADE:
                sql_server.update(item->text(1).toInt(), item->parent()->text(1).toInt(), item->text(0), item->parent()->text(0), item->text(2).toFloat());
                break;
            default:
                break;
        }
        isediting = false;
    }
}

void lesson_manage_page::on_read_file_btn_clicked()
{
    my_browser = new file_browser();
    connect(my_browser, SIGNAL(send_file_read_signal()), this, SLOT(refresh()));
}

