/******************************************************************************
  * @file    login_page.cpp
  * @author  Jialiang Li
  * @brief   member function of class loogin_page, including all the login operation and login page UI
  *
*/
#pragma execution_character_set("utf-8")

#include "login_page.h"
#include "ui_login_page.h"

login_page::login_page(QWidget *parent)
/**
  * @brief 登录页面构造函数
  * @param
  * @retval
  */
    :QWidget(parent),
    ui(new Ui::login_page)
{
    ui->setupUi(this);
    init();
}

login_page::~login_page()
/**
  * @brief  登录界面析构函数，关闭登录界面UI
  * @param
  * @retval
  */
{
    delete ui;
}

void login_page::init()
/**
  * @brief  初始化，建立连接
  * @param
  * @retval void
  */
{

    //connect(&lesson_manage,SIGNAL(send_turn2student_manage_signal()),this,SLOT(do_process_turn_back_request()));
}
void login_page::on_btn_login_clicked()
/**
  * @brief  登录按钮响应。在数据库中查找账号和密码，尝试登录
  * @param
  * @retval void
  */
{
    //如果登录成功，发送成功信号
    if(try_login())
    {
        emit send_login_success_signal(ui->login_username->text());
    }
}

void login_page::on_btn_quit_clicked()
/**
  * @brief  登录界面退出按钮响应，关闭登录界面
  * @param
  * @retval void
  */
{
    exit(0);
}


bool login_page::try_login()
{
    QString name=ui->login_username->text();
    QString passwd=ui->login_passwd->text();
    return sql_server.check_passwd(name,passwd);
}
