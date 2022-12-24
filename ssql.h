/******************************************************************************
  * @file    ssql.h
  * @author  Jialiang Li
  * @brief   class: sSql
  *          info:  this project uses MySql database, all basic Sql operations are defined in this file
  *
*/
#ifndef SSQL_H
#define SSQL_H

#include <QObject>
#include "add_student.h"

class sSql : public QObject
{
    Q_OBJECT
public:
    explicit sSql(QObject *parent = nullptr);
    bool test_connection();
    //bool read_student(QString student_id);
    bool read_student();
    bool add_student();
    bool add_grade();
private:
    bool add_student_base();
    bool add_grade_base();
signals:



};

#endif // SSQL_H
