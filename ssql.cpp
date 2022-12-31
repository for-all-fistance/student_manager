/******************************************************************************
  * @file    ssql.cpp
  * @author  Jialiang Li
  * @brief   member function of class ssql,including all the sql operations, such as  add, search...
  *
*/
#pragma execution_character_set("utf-8")

#include "ssql.h"


/**
 * @brief size 获得一个QSqlQuery中数据的条数
 * @param q query to search
 * @return
 */
int size(QSqlQuery q)
{
    q.last();
    int col=q.at()+1;
    return col;
}


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
 * @brief sSql::find_lesson 查找对应的课程ID
 * @param lesson_name 要查找的课程名称
 * @param year 要查找的课程学年
 * @param term 要查找的课程学期
 * @return 课程对应的课程ID，没有找到返回-1
 */
int sSql::find_lesson(QString lesson_name,QString year,QString term)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    QString find = QString("SELECT * FROM student_info WHERE "
                           "lesson_name = :lesson_name "
                           "year = :year "
                           "term = :term ");// sql语句字符串
    query.bindValue(":lesson_name", lesson_name);//使用 bindValue函数将占位符替换
    query.bindValue(":year",year);
    query.bindValue(":term",term);
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
    query.prepare("INSERT INTO student_info (stu_name,stud_id,class) VALUES (:stu_name,:stud_id,:class)");
    query.bindValue(":stud_id", stud_id);
    query.bindValue(":stu_name", stu_name);
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
    class add_student *new_student;
    new_student=new class add_student;//创建界面对象
    new_student->setWindowTitle("new student");//标题
    new_student->show();
    connect(new_student,SIGNAL(send_new_student_signal(int,QString,QString)),this,
            SLOT(try_add_student(int,QString,QString)));//连接信号和槽函数
    return true;
}

/**
 * @brief sSql::try_add_student 处理信号
 * @param stud_id
 * @param stu_name
 * @param _class
 */
void sSql::try_add_student(int stud_id,QString stu_name,QString _class)
{
    if(add_student_base(stud_id,stu_name,_class))
        emit send_student_added_signal();
}

/**
 * @brief sSql::read_all_student
 * @return true if success.
 */
bool sSql::read_all_student(QTreeWidget *content)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM student_info");
    if(!query.exec())
    {
        qDebug() << "Error: Fail to query table. " << query.lastError();
        return false;
    }
    else
    {
        int col=4;
        content->setColumnCount(col);
        QStringList header({"姓名","学号","班级","优良程度"});
        content->setHeaderLabels(header);
        while(query.next())
        {
            QString stud_id = query.value("stud_id").toString();
            QString stud_name = query.value("stu_name").toString();
            QString _class = query.value("class").toString();
            QString performance=query.value("performance").toString();
            qDebug()<<QString("id:%1    name:%2    class:%3   performance:%4").arg(stud_id).arg(stud_name).arg(_class).arg(performance);
            QTreeWidgetItem* new_top_item = new QTreeWidgetItem(QStringList({stud_name,stud_id,_class,performance}));//创建层
            content->addTopLevelItem(new_top_item);//添加层
            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");
            query1.bindValue(":stud_id",stud_id);
            query1.exec();
            while(query1.next())
            {
                QString lesson_name=query1.value("lesson_name").toString();
                QString grade=query1.value("grade").toString();
                QTreeWidgetItem* sec_child_item = new QTreeWidgetItem(QStringList{lesson_name,grade});//创建子节点
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        content->update();
        return true;
    }
}

/**
 * @brief sSql::search_for_student
 * @param stu_name
 * @param widget_to_show
 * @return
 */
bool sSql::search_for_student(QString stu_name,QTreeWidget *widget_to_show)
{
    widget_to_show->clear();
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM student_info WHERE stu_name = :stu_name");//要执行的sql语句
    query.bindValue(":stu_name",stu_name);
    if(!query.exec())
    {
        qDebug() << "Error: fail to find student. " << query.lastError();
        return false;
    }
    else
    {
        int col =4;
        widget_to_show->setColumnCount(col);
        while(query.next())
        {
            QString stud_id = query.value("stud_id").toString();
            QString stud_name = query.value("stu_name").toString();
            QString _class = query.value("class").toString();
            QString performance=query.value("performance").toString();
            QTreeWidgetItem* new_top_item = new QTreeWidgetItem(QStringList({stud_name,stud_id,_class,performance}));//创建层
            widget_to_show->addTopLevelItem(new_top_item);//添加层
            QSqlQuery query1(mydb);//创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");
            query1.bindValue(":stud_id",stud_id);
            query1.exec();
            while(query1.next())
            {
                QString lesson_name=query1.value("lesson_name").toString();
                QString grade=query1.value("grade").toString();
                QTreeWidgetItem* sec_child_item = new QTreeWidgetItem(QStringList{lesson_name,grade});//创建子节点
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        widget_to_show->update();
        return true;
    }
}
/**
 * @brief sSql::add_grade
 * @return
 */
bool sSql::add_grade()
{
    class add_score *new_score;
    new_score=new class add_score;//创建界面对象
    new_score->setWindowTitle("new grade");//标题
    new_score->show();
    connect(new_score,SIGNAL(send_new_score_signal(QString,QString,QString,QString,float)),this,
            SLOT(try_add_grade(QString,QString,QString,QString,float)));//连接信号和槽函数
    return true;
}

/**
 * @brief sSql::try_add_grade 处理信号
 * @param stu_name
 * @param lesson_name
 * @param year
 * @param term
 * @param grade
 */
void sSql::try_add_grade(QString stu_name,QString lesson_name,QString year,QString term,float grade)
{
    int stu_id=find_student(stu_name);
    int lesson_id=find_lesson(lesson_name,year,term);
    if(add_garde_base(stu_id,lesson_id,grade))
        emit send_grade_added_signal();
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

/**
 * @brief sSql::check_passwd
 * @param user_name
 * @param passwd
 * @return
 */
bool sSql::check_passwd(QString user_name,QString passwd)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id",user_name);
    query.exec();
    int s=query.at();
    query.next();
    if(s==-2)//没有找到user
    {
        qDebug() << "Error: user not found" << query.lastError();
        QMessageBox::critical(nullptr, QObject::tr("error"),
                              QObject::tr("user not found\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else if(passwd==query.value("passwd").toString())//找到user且密码正确
    {
        qDebug()<<"login success!";
        return true;
    }
    else//密码错误
    {
        QMessageBox::critical(nullptr, QObject::tr("error"),
                              QObject::tr("password incorrect\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
}

/**
 * @brief sSql::delete_student 删除学号为stud_id的学生
 * @param stud_id 要删除学生的学号
 * @return
 */
bool sSql::delete_student_base(int stud_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("DELETE FROM student_info WHERE stud_id = :stud_id");//要执行的sql语句
    query.bindValue(":stud_id",stud_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to delete student. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"student deleted!";
        return true;
    }
    return true;
}

/**
 * @brief sSql::delete_student 重载，删除姓名为stu_name的学生
 * @param stu_name 姓名
 * @return
 */
bool sSql::delete_student_base(QString stu_name)
{
    int lesson_id=find_student(stu_name);
    if(lesson_id!=-1)
        return delete_student_base(lesson_id);
    else
        return false;
}

/**
 * @brief sSql::delete_grade 删除，学号为stud_id的学生，课程ID为lesson_id的成绩
 * @param stud_id 学号
 * @param lesson_id 课程ID
 * @return
 */
bool sSql::delete_grade_base(int stud_id,int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("DELETE FROM grade WHERE "
                  "stud_id = :stud_id "
                  "lesson_id = :lesson_id");//要执行的sql语句
    query.bindValue(":stud_id",stud_id);
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to delete grade. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"grade deleted!";
        return true;
    }
    return true;
}

/**
 * @brief sSql::delete_grade 重载
 * @param stu_name
 * @param lesson_name
 * @param year
 * @param term
 * @return
 */
bool sSql::delete_grade_base(QString stu_name,QString lesson_name,QString year,QString term)
{
    int lesson_id=find_lesson(lesson_name,year,term);
    int stud_id=find_student(stu_name);
    if(lesson_id!=-1&&stud_id!=-1)
        return delete_grade_base(stud_id,lesson_id);
    else
        return false;
}

/**
 * @brief sSql::delete_lesson 删除ID为lesson_id的课程
 * @param lesson_id
 * @return true if success
 */
bool sSql::delete_lesson_base(int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("DELETE FROM lesson_info WHERE lesson_id = :lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to delete lesson. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"lesson deleted!";
        return true;
    }
}

/**
 * @brief sSql::delete_lesson 重载，删除课程
 * @param lesson_name
 * @param year
 * @param term
 * @return
 */
bool sSql::delete_lesson_base(QString lesson_name,QString year,QString term)
{
    int lesson_id=find_lesson(lesson_name,year,term);
    if(lesson_id!=-1)//课程存在
        return delete_lesson_base(lesson_id);
    else
        return false;
}

/**
 * @brief sSql::delete_student 提供给其他模块的接口函数，
 * @return true if success
 */
bool sSql::delete_student()
{
    class delete_student *delete_stu;
    delete_stu=new class delete_student;
    delete_stu->setWindowTitle("delete student");
    delete_stu->show();
    connect(delete_stu,SIGNAL(send_delete_student_signal(int)),this,
            SLOT(try_delete_student(int)));//连接信号和槽函数
    connect(delete_stu,SIGNAL(send_delete_student_signal(QString)),this,
            SLOT(try_delete_student(QString)));
    return true;
}

/**
 * @brief sSql::try_delete_student
 * @param stud_id
 */
void sSql::try_delete_student(int stud_id)
{
    if(delete_student_base(stud_id))
        emit send_student_deleted_signal();
}

/**
 * @brief sSql::try_delete_student
 * @param stu_name
 */
void sSql::try_delete_student(QString stu_name)
{
    if(delete_student_base(stu_name))
        emit send_student_deleted_signal();
}

bool sSql::delete_grade()
{
    class delete_grade *del_grade;
    del_grade=new class delete_grade;
    del_grade->setWindowTitle("delete grade");
    del_grade->show();
    connect(del_grade,SIGNAL(send_grade_deleted_signal(int,int)),this,SLOT(try_delete_grade(int,int)));
    connect(del_grade,SIGNAL(send_grade_deleted_signal(QString,QString,QString,QString)),this,SLOT(try_delete_grade(QString,QString,QString,QString)));
    return true;
}

/**
 * @brief sSql::try_delete_grade
 * @param stud_id
 * @param lesson_id
 */
void sSql::try_delete_grade(int stud_id,int lesson_id)
{
    if(delete_grade_base(stud_id,lesson_id))
        emit send_grade_deleted_signal();
}

/**
 * @brief sSql::try_delete_grade
 * @param stu_name
 * @param lesson_name
 * @param year
 * @param term
 */
void sSql::try_delete_grade(QString stu_name,QString lesson_name,QString year,QString term)
{
    if(delete_grade_base(stu_name,lesson_name,year,term))
        emit send_grade_deleted_signal();
}





