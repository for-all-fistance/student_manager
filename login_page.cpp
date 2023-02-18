/******************************************************************************
  * @file    login_page.cpp
  * @author  Jialiang Li
  * @brief   member function of class loogin_page, including all the login operation and login page UI
  *
*/
#pragma execution_character_set("utf-8")

#include "login_page.h"
#include "ui_login_page.h"
#include "global.h"

login_page::login_page(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::login_page)
{
    ui->setupUi(this);
    sql_server = new sSql;
    this->show();
}

login_page::~login_page()
{
    delete ui;
    delete sql_server;
}

/**
 * @brief login_page::on_btn_login_clicked 登录按钮响应。在数据库中查找账号和密码，尝试登录
 */
void login_page::on_btn_login_clicked()
{
    QString name = ui->login_username->text();
    //如果登录成功，发送成功信号
    if (try_login())
    {
        set_current_user(name);
        emit send_login_success_signal();
    }
}

/**
 * @brief login_page::on_btn_quit_clicked 登录界面退出按钮响应，关闭登录界面
 */
void login_page::on_btn_quit_clicked()
{
    exit(0);
}

/**
 * @brief login_page::try_login 尝试登录
 * @return true for success
 */
bool login_page::try_login()
{
    QString name = ui->login_username->text();
    QString passwd = ui->login_passwd->text();
    return sql_server->check_passwd(name, passwd);
}

