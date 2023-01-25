﻿/******************************************************************************
  * @file    ssql.h
  * @author  Jialiang Li
  * @brief   class: sSql
  *          info:  this project uses MySql database, all basic Sql operations are defined in this file
  *
*/
#ifndef SSQL_H
#define SSQL_H

#include "qsqldatabase.h"
#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QErrorMessage>
#include <qmessagebox.h>
#include "add_student.h"
#include "add_score.h"
#include "delete_student.h"
#include "qtreewidget.h"
#include "delete_grade.h"


class sSql : public QObject
{
    Q_OBJECT
public:
    explicit sSql(QObject *parent = nullptr);
    bool read_all_student(QTreeWidget *content);
    bool calc_performance();
    bool calc_performance(int stud_id);
    bool search_for_student(QString stu_name,QTreeWidget *widget_to_show);
    bool add_student();
    bool add_grade();
    bool check_passwd(QString user_name,QString passwd);
    bool delete_grade(int stud_id,int lesson_id);
    bool delete_grade(QString stu_name,QString lesson_name,QString year,QString term);
    bool delete_lesson(int lesson_id);
    bool delete_lesson(QString lesson_name,QString year,QString term);
    bool delete_student(int stud_id);
    bool delete_student(QString stu_name);
    bool update(QString stu_name,int stud_id,QString _class,QString performance);
    bool update(int stud_id,int lesson_id,QString stu_name,QString lesson_name,float grade);
    bool update(int lesson_id,QString lesson_name,QString year,QString term);
    bool update(QString stu_name,QString lesson_name,float grade,QString year,QString term);

private:
    bool add_student_base(int stud_id,QString stu_name,QString _class);
    bool add_garde_base(int stu_id,int lesson_id,float grade);
    QSqlDatabase mydb;
    bool create_connection();
    QString find_student(int stud_id);
    int find_student(QString stu_name);
    QString find_lesson(int lesson_id);
    int find_lesson(QString lesson_name,QString year,QString term);
    float average_score(int stud_id);
signals:
    void send_student_deleted_signal();
    void send_student_added_signal();
    void send_grade_added_signal();
    void send_grade_deleted_signal();
    void send_lesson_deleted_signal();
    void send_lesson_added_signal();

private slots:
    void try_add_student(int stud_id,QString stu_name,QString _class);
    void try_add_grade(QString stu_name,QString lesson_name,QString year,QString term,float grade);
    void try_delete_student(int stud_id);
    void try_delete_student(QString stu_name);
    void try_delete_grade(int stud_id,int lesson_id);
    void try_delete_grade(QString stu_name,QString lesson_name,QString year,QString term);

};

#endif // SSQL_H
