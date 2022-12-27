/******************************************************************************
  * @file    ssql.cpp
  * @author  Jialiang Li
  * @brief   member function of class ssql,including all the sql operations, such as  add, search...
  *
*/
#include "ssql.h"


sSql::sSql(QObject *parent)
    : QObject{parent}
{
    create_connection();
}

/**
 * @brief sSql::create_connection 测试数据库是否联通
 * @return 联通则返回true,否则返回false
 */
bool sSql::create_connection()
{
    // 创建一个数据库连接，指定数据库驱动
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    // 数据库连接需要设置的信息
    //mydb.setDatabaseName((qApp->applicationDirPath() + "\\data.db"));
    mydb.setDatabaseName("F:\\QTProjectRepo\\student_manager-master\\data.db");
     if (!mydb.open()) {
         QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
             QObject::tr("Unable to establish a database connection.\n"
                         "This example needs SQLite support. Please read "
                         "the Qt SQL driver documentation for information how "
                         "to build it.\n\n"
                         "Click Cancel to exit."), QMessageBox::Cancel);
         return false;
     }
    return true;
}

/**
 * @brief sSql::find_student 查找学号为stud_id的学生姓名
 * @param stud_id 要查询的学号
 * @return 对应的学生姓名，如果没有找到则返回空串
 */
QString sSql::find_student(int stud_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    QString find = QString("SELECT * FROM student_info WHERE stud_id = :stud_id");// sql语句字符串
    query.bindValue(":stud_id", stud_id);//使用 bindValue函数将占位符替换
    query.prepare(find);
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find student. " << query.lastError();
        return "";
    }
    else
    {     qDebug() << "student found!";
        QString stu_name=query.value("stu_name").toString();
        return stu_name;
    }
}

/**
 * @brief sSql::find_student 查找姓名为stu_name的学生学号，如果有重名则只会返回第一个
 * @param stu_name 要查找的学生姓名
 * @return 对应的学号，如果没有找到则返回-1
 */
int sSql::find_student(QString stu_name)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    QString find = QString("SELECT * FROM student_info WHERE stu_name = :stu_name");// sql语句字符串
    query.bindValue(":stu_name", stu_name);//使用 bindValue函数将占位符替换
    query.prepare(find);
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find student. " << query.lastError();
        return -1;
    }
    else
    {     qDebug() << "student found!";
        int stud_id=query.value("stud_id").toInt();
        return stud_id;
    }
}

/**
 * @brief sSql::find_lesson 查找课程ID为lesson_id的课程名称
 * @param lesson_id 要查找的课程ID
 * @return 对应的课程名称，如果没有找到则返回空串
 */
QString sSql::find_lesson(int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    QString find = QString("SELECT * FROM lesson_info WHERE lesson_id = :lesson_id");// sql语句字符串
    query.bindValue(":lesson_id", lesson_id);//使用 bindValue函数将占位符替换
    query.prepare(find);
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find lesson. " << query.lastError();
        return "";
    }
    else
    {     qDebug() << "lesson found!";
        QString lesson_name=query.value("lesson_name").toString();
        return lesson_name;
    }
}


/**
 * @brief sSql::find_lesson 查找课程对应的课程ID
 * @param lesson_name 要查找的课程名称
 * @param year 要查找的课程学年
 * @param term 要查找的课程学期
 * @return 对应的课程名称
 */
int sSql::find_lesson(QString lesson_name,QString year,QString term)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    QString find = QString("SELECT * FROM student_info WHERE "
                           "lesson_name = :lesson_name "
                           "year = :year "
                           "term = :term ");// sql语句字符串
    query.bindValue(":lesson_name", lesson_name);//使用 bindValue函数将占位符替换
    query.prepare(find);
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find student. " << query.lastError();
        return -1;
    }
    else
    {     qDebug() << "student found!";
        int stu_id=query.value("stud_id").toInt();
        return stu_id;
    }
}

/**
 * @brief sSql::add_student_base
 * @param stud_id
 * @param stu_name
 * @param _class
 * @return
 */
bool sSql::add_student_base(int stud_id,QString stu_name,QString _class)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("INSERT INTO student_info VALUES (:stu_name,:stud_id,:class)");
    query.bindValue(":stud_id", stud_id);
    query.bindValue(":stud_name", stu_name);
    query.bindValue(":class", _class);
    if(!query.exec())
    {
        qDebug() << "Error: Fail to add new student. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"a new student added";
        return true;
    }
}


/**
 * @brief sSql::add_student 向数据库中加入一个学生的数据,接口函数，唤起一个界面来获取用户输入。
 * @return 成功则返回true,否则返回false
 */
bool sSql::add_student()
{
    class add_student new_student;
    new_student.show();
    /*while(!new_student.isHidden())//阻塞
    {
        QTime dieTime= QTime::currentTime().addMSecs(5);
        while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    */

    return add_student_base(new_student.get_id(),new_student.get_name(),new_student.get_class());
}

/**
 * @brief sSql::read_all_student
 * @return true if success.
 */
bool sSql::read_all_student()
{
        QSqlQuery query(mydb);
        //query.exec();
        if(!query.exec("SELECT * FROM student_info"))
        {
            qDebug() << "Error: Fail to query table. " << query.lastError();
            return false;
        }
        else
        {
            while(query.next())
            {
                int stud_id = query.value(0).toInt();
                QString stud_name = query.value(1).toString();
                QString _class = query.value(2).toString();
                qDebug()<<QString("id:%1    name:%2    class:%3").arg(stud_id).arg(stud_name).arg(_class);
            }
            return true;
        }
}

/**
 * @brief sSql::add_grade
 * @return
 */
bool sSql::add_grade()
{
    class add_score new_score;
    new_score.show();
    while(!new_score.isHidden())//阻塞
    {
        QTime dieTime= QTime::currentTime().addMSecs(5);
        while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    int stu_id=find_student(new_score.get_lesson_name());
    int lesson_id=find_lesson(new_score.get_lesson_name(),new_score.get_year(),new_score.get_term());
    return add_garde_base(stu_id,lesson_id,new_score.get_score());
}


/**
 * @brief sSql::add_garde_base
 * @param stu_id
 * @param lesson_id
 * @param grade
 * @return
 */
bool sSql::add_garde_base(int stu_id,int lesson_id,float grade)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("INSERT INTO grade VALUES (:stud_id,:lesson_id,:stu_name,:lesson_name,:grade)");
    query.bindValue(":stud_id",stu_id);
    query.bindValue("lesson_id",lesson_id);
    query.bindValue(":stu_name",find_student(stu_id));
    query.bindValue(":lesson_name",find_lesson(lesson_id));
    query.bindValue(":grade",grade);
    if(!query.exec())
    {
        qDebug() << "Error: fail to add new score. " << query.lastError();
        return false;
    }
    else
    {
            qDebug()<<"new score added!";
            return true;
    }
}
