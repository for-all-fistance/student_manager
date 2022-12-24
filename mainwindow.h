﻿/******************************************************************************
  * @file    mainwindow.h
  * @author  Jialiang Li
  * @brief   class: MainWindow
  *          info:  main window contains others page as its member,
  *
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "login_page.h"
#include "lesson_manage_page.h"
#include "ssql.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_exit_clicked();
    void on_btn_relogin_clicked();
    void set_content();
    void on_turn2lesson_manage_clicked();
    void do_process_turn_back_request();
    void do_process_login_request();

    void on_add_stu_clicked();

private:
    Ui::MainWindow *ui;
    void init();
    login_page login;
    lesson_manage_page lesson_manage;
    sSql sql_server;
};
#endif // MAINWINDOW_H
