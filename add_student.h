/******************************************************************************
  * @file    add_student.h
  * @author  Jialiang Li
  * @brief   添加学生的窗口
  *
*/
#ifndef ADD_STUDENT_H
#define ADD_STUDENT_H

#include <QWidget>

namespace Ui {
class add_student;
}

class add_student : public QWidget
{
    Q_OBJECT

public:
    explicit add_student(QWidget *parent = nullptr);
    ~add_student();
    QString get_name();
    int get_id();
    QString get_class();
private slots:

    void on_new_stud_btn_clicked();

    void on_cancel_clicked();

signals:
    void send_new_student_signal();
    void send_cancel_signal();
private:
    Ui::add_student *ui;
    int score_count=0;
};

#endif // ADD_STUDENT_H
