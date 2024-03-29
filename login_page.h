﻿/******************************************************************************
  * @file    login_page.h
  * @author  Jialiang Li
  * @brief   class: login_page
  *          info:  interface for user to login
  *
*/
#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include "ssql.h"
#include <QWidget>

namespace Ui
{
    class login_page;
}

class login_page : public QWidget
{
    Q_OBJECT

public:
    explicit login_page(QWidget *parent = nullptr);
    ~login_page();

private slots:
    void on_btn_login_clicked();
    void on_btn_quit_clicked();

signals:
    void send_login_success_signal();

private:
    Ui::login_page *ui;
    sSql *sql_server;
    bool try_login();
};

#endif // LOGIN_PAGE_H
