/******************************************************************************
  * @file    login_fail_page.cpp
  * @author  Jialiang Li
  * @brief   member function of class login_fail_page, when you fail to login, a login_fail_page will show up
  *
*/
#include "login_fail_page.h"
#include "ui_login_fail_page.h"

login_fail_page::login_fail_page(QWidget *parent) :
/**
  * @brief  登录失败界面初始化函数
  * @param
  * @retval
  */
    QWidget(parent),
    ui(new Ui::login_fail_page)
{
    ui->setupUi(this);
}

login_fail_page::~login_fail_page()
/**
  * @brief  登陆失败界面析构函数
  * @param
  * @retval
  */
{
    delete ui;
}

void login_fail_page::on_relogin_clicked()
/**
  * @brief  登陆失败界面确认按钮，隐藏登录失败弹窗
  * @param
  * @retval
  */
{
    this->hide();
}

