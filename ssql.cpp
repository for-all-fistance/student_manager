﻿/******************************************************************************
  * @file    ssql.cpp
  * @author  Jialiang Li
  * @brief   member function of class ssql,including all the sql operations, such as  add, search...
  *
*/
#include "ssql.h"
#include <QtSql>
#include <QDebug>
#include <QErrorMessage>
#include <qmessagebox.h>

sSql::sSql(QObject *parent)
    : QObject{parent}
{
    test_connection();
}

/**
 * @brief sSql::test_connection 测试数据库是否联通
 * @return 联通则返回true,否则返回false
 */
bool sSql::test_connection()
{
    // 创建一个数据库连接，指定数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // 数据库连接需要设置的信息
    db.setDatabaseName("data");
     if (!db.open()) {
         QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
             QObject::tr("Unable to establish a database connection.\n"
                         "This example needs SQLite support. Please read "
                         "the Qt SQL driver documentation for information how "
                         "to build it.\n\n"
                         "Click Cancel to exit."), QMessageBox::Cancel);
         return false;
     }
    // 测试完毕，关闭数据库
     db.close();
    return true;
}

/**
 * @brief sSql::add_student 向数据库中加入一个学生的数据,接口函数，唤起一个界面来获取用户输入。
 * @return 联通则返回true,否则返回false
 */
bool sSql::add_student()
{
    class add_student new_student;
    new_student.show();
    while(!new_student.isHidden())//阻塞
    {
        QTime dieTime= QTime::currentTime().addMSecs(5);
        while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    return true;
}



