/******************************************************************************
  * @file    add_student.h
  * @author  Jialiang Li
  * @brief   add_student类，用来添加学生信息
  *
*/
#ifndef ADD_STUDENT_H
#define ADD_STUDENT_H

#include <QWidget>

namespace Ui
{
    class add_student;
}

class add_student : public QWidget
{
    Q_OBJECT

public:
    explicit add_student(QWidget *parent = nullptr);
    ~add_student();
private slots:

    void on_new_stud_btn_clicked();

    void on_cancel_btn_clicked();

signals:
    void send_new_student_signal(int stud_id, QString stu_name, QString _class);
private:
    Ui::add_student *ui;
    int score_count = 0;
};

#endif // ADD_STUDENT_H
