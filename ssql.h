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


class sSql : public QObject
{
    Q_OBJECT
public:
    explicit sSql(QObject *parent = nullptr);
    bool read_all_student();
    QString find_student(int stud_id);
    int find_student(QString stu_name);
    QString find_lesson(int lesson_id);
    int find_lesson(QString lesson_name,QString year,QString term);
    bool add_student();
    bool add_grade();
private:
    bool add_student_base(int stud_id,QString stu_name,QString _class);
    bool add_garde_base(int stu_id,int lesson_id,float grade);
    QSqlDatabase mydb;
    bool create_connection();
signals:



};

#endif // SSQL_H
