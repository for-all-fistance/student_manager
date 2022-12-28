/******************************************************************************
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
#include "qtreewidget.h"


class sSql : public QObject
{
    Q_OBJECT
public:
    explicit sSql(QObject *parent = nullptr);
    bool read_all_student(QTreeWidget *content);
    QString find_student(int stud_id);
    int find_student(QString stu_name);
    QString find_lesson(int lesson_id);
    int find_lesson(QString lesson_name,QString year,QString term);
    bool search_for_student(QString stu_name,QTreeWidget *widget_to_show);
    bool add_student();
    bool add_grade();
    bool check_passwd(QString user_name,QString passwd);
    bool delete_student();
    bool delete_grade();
private:
    bool add_student_base(int stud_id,QString stu_name,QString _class);
    bool add_garde_base(int stu_id,int lesson_id,float grade);
    QSqlDatabase mydb;
    bool create_connection();
signals:

private slots:
    void try_add_student(int stud_id,QString stu_name,QString _class);
    void try_add_grade(QString stu_name,QString lesson_name,QString year,QString term,float grade);
};

#endif // SSQL_H
