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

private slots:
    void on_new_stud_clicked();

private:
    Ui::add_student *ui;
    int score_count=0;
};

#endif // ADD_STUDENT_H
