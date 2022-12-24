/******************************************************************************
  * @file    login_fail_page.h
  * @author  Jialiang Li
  * @brief   class: login_fail_page
  *          info:  when you fail to login, this page will show up
  *
*/
#ifndef LOGIN_FAIL_PAGE_H
#define LOGIN_FAIL_PAGE_H

#include <QWidget>

namespace Ui {
class login_fail_page;
}

class login_fail_page : public QWidget
{
    Q_OBJECT

public:
    explicit login_fail_page(QWidget *parent = nullptr);
    ~login_fail_page();

private slots:
    void on_relogin_clicked();

private:
    Ui::login_fail_page *ui;
};

#endif // LOGIN_FAIL_PAGE_H
