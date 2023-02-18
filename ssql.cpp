/******************************************************************************
 * @file    ssql.cpp
 * @author  Jialiang Li
 * @brief   member function of class ssql,including all the sql operations, such as  add, search...
 *
*/
#pragma execution_character_set("utf-8")

#include "ssql.h"
#include "global.h"
#include "add_student.h"
#include "add_score.h"
#include "add_lesson.h"

/**
 * @brief size 计算一个QSqlQuery中数据的条数
 * @param q query to search
 * @return
 */
int size(QSqlQuery &q)
{
    q.last();
    int col = q.at() + 1;
    return col;
}


sSql::sSql(QObject *parent)
    : QObject{parent}
{
    create_connection();
}

sSql::~sSql()
{
    mydb.close();
}

/**
 * @brief sSql::create_connection 测试数据库是否联通
 * @return 联通则返回true,否则返回false
 */
bool sSql::create_connection()
{
    // 创建一个数据库连接，指定数据库驱动
    mydb = QSqlDatabase::addDatabase("QSQLITE", "connection time :" + QTime::currentTime().toString());
    qDebug() << mydb.connectionName();
    // 数据库连接需要设置的信息
    mydb.setDatabaseName(qApp->applicationDirPath() + "\\data.db");
    //mydb.setDatabaseName("F:\\QTProjectRepo\\student_manager-master\\data.db");
    if (!mydb.open())
    {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
            QObject::tr("Unable to establish a database connection.\n"
                "This example needs SQLite support. Please read "
                "the Qt SQL driver documentation for information how "
                "to build it.\n"
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
    query.prepare(find);
    query.bindValue(":stud_id", stud_id);//使用 bindValue函数将占位符替换
    if (!query.exec()) // 执行sql语句
    {
        QMessageBox::critical(nullptr, QString("error"),
            QString("Error: Fail to find student." + query.lastError().text() +
                "Click Cancel to exit."), QMessageBox::Cancel);
        return "";
    }
    else if (query.first())
    {
        qDebug() << "student found!";
        QString stu_name = query.value("stu_name").toString();
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
    query.prepare("SELECT * FROM student_info WHERE stu_name=:stu_name");
    query.bindValue(":stu_name", stu_name); //使用 bindValue函数将占位符替换
    if (!query.exec()) // 执行sql语句
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to find student." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else if (query.first())
    {
        int stud_id = query.value("stud_id").toInt();
        qDebug() << "student found!";
        return stud_id;
    }
    else
    {
        return -1;
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
    query.prepare("SELECT * FROM lesson_info WHERE lesson_id=:lesson_id");
    query.bindValue(":lesson_id", lesson_id);//使用 bindValue函数将占位符替换
    if (!query.exec()) // 执行sql语句
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to find lesson." + query.lastError().text() +
                "Click Cancel to exit."), QMessageBox::Cancel);
        return "";
    }
    else if (query.first())
    {
        qDebug() << "lesson found!";
        QString lesson_name = query.value("lesson_name").toString();
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
int sSql::find_lesson(QString lesson_name, QString year, QString term)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info WHERE lesson_name = :lesson_name AND year = :year AND term = :term");
    query.bindValue(":lesson_name", lesson_name);//使用 bindValue函数将占位符替换
    query.bindValue(":year", year);
    query.bindValue(":term", term);
    if (!query.exec()) // 执行sql语句
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to find lesson." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else if (query.next())
    {
        qDebug() << "lesson found!";
        int stu_id = query.value("lesson_id").toInt();
        return stu_id;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief sSql::find_lesson 尝试获取对应的课程号，可能出现误查
 * @param lesson_name
 * @return
 */
int sSql::find_lesson(QString lesson_name)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info WHERE "
        "lesson_name = :lesson_name ");
    query.bindValue(":lesson_name", lesson_name);//使用 bindValue函数将占位符替换
    if (!query.exec()) // 执行sql语句
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to find lesson." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else if (query.next())
    {
        qDebug() << "lesson found!";
        int lesson_id = query.value("lesson_id").toInt();
        return lesson_id;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief sSql::calc_performance 重新计算所有人的优良程度
 * @return
 */
bool sSql::calc_performance()
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM student_info");//要执行的sql语句
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to find student when calculating perfoemance." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "ready to calculate performance.";
        while (query.next())
        {
            calc_performance(query.value("stud_id").toInt());
        }
        qDebug() << "calculation finished!";
        return true;
    }
}

/**
 * @brief sSql::calc_performance 重新计算某个学生的平均分和优良程度
 * @param stud_id 需要重新计算的学生的学号
 * @return
 */
bool sSql::calc_performance(int stud_id)
{
    float avrg = student_average(stud_id);
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("UPDATE student_info SET performance=:performance,average_score=:average_score WHERE stud_id = :stud_id");//要执行的sql语句
    query.bindValue(":stud_id", stud_id);
    if (avrg >= 90)
    {
        query.bindValue(":performance", "优");
    }
    else if (avrg >= 80)
    {
        query.bindValue(":performance", "良");
    }
    else if (avrg >= 60)
    {
        query.bindValue(":performance", "合格");
    }
    else
    {
        query.bindValue(":performance", "不合格");
    }
    query.bindValue(":average_score", avrg); //更新成绩
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to calculate performance of student." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief sSql::student_average 计算某个学生的平均分
 * @param stud_id 要计算的学生学号
 * @return 如果计算正确就返回平均分，否则返回-1（包括该学生暂无成绩）
 */
float sSql::student_average(int stud_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");//要执行的sql语句，获取该学生的所有成绩
    query.bindValue(":stud_id", stud_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to calculate average score." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else
    {
        float sum = 0;
        int count = 0;
        while (query.next())
        {
            sum += query.value("grade").toFloat();
            count += 1;
        }
        if (count == 0)
        {
            return -1;    //成绩为空
        }
        else
        {
            return sum / count;
        }
    }
}

/**
 * @brief sSql::clac_lesson_summery重新计算所有课程的概况
 * @return
 */
bool sSql::clac_lesson_summery()
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info");//要执行的sql语句
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error when trying to calculate lesson summery:" + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "ready to calculate lesson summery.";
        while (query.next())
        {
            calc_lesson_summery(query.value("lesson_id").toInt());
        }
        qDebug() << "calculation finished";
        return true;
    }
}

/**
 * @brief sSql::calc_lesson_summery 重新计算课程的概况
 * @param lesson_id 要计算课程的课程号
 * @return
 */
bool sSql::calc_lesson_summery(int lesson_id)
{
    float pass_rt = 0;
    float avrg_score = 0;
    int perfect = 0;
    int good = 0;
    int fail = 0;
    int qualified = 0;
    int total_cnt = 0;
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM grade WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("rror when trying to find leson:" + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        int pass = 0;
        float sum = 0;
        int count = 0;
        while (query.next())
        {
            float grade = query.value("grade").toFloat();
            sum += grade;
            pass += grade >= 60 ? 1 : 0;
            perfect += grade >= 90 ? 1 : 0;
            good += grade >= 80 && grade < 90 ? 1 : 0;
            qualified += grade >= 60 && grade < 80 ? 1 : 0;
            fail += grade < 60 ? 1 : 0;
            count += 1;
        }
        if (count != 0) //成绩不为空
        {
            pass_rt = float(pass) / count;
            avrg_score = sum / count;
            total_cnt = count;
        }
    }
    query.prepare("UPDATE lesson_info SET total_count=:total_cnt,average_score=:avrg_score,pass_rate=:pass_rt,"
        "perfect=:perfect,good=:good,fail=:fail,qualified=:qualified "
        "WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":total_cnt", total_cnt);
    query.bindValue(":avrg_score", avrg_score);
    query.bindValue(":pass_rt", pass_rt * 100);
    query.bindValue(":perfect", perfect);
    query.bindValue(":good", good);
    query.bindValue(":fail", fail);
    query.bindValue(":qualified", qualified);
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: in sSql::calc_lesson_summery(int lesson_id)." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        return true;
    }
}


/**
 * @brief sSql::lesson_average 计算某门课程的平均分
 * @param lesson_id 要计算的课程号
 * @return  如果计算正确就返回平均分，否则返回-1（包括该课程暂无成绩）
 */
float sSql::lesson_average(int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM grade WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error when trying to find leson:" + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else
    {
        float sum = 0;
        int count = 0;
        while (query.next())
        {
            sum += query.value("grade").toFloat();
            count += 1;
        }
        qDebug() << "lesson average score calculation finished";
        if (count == 0)
        {
            return -1;    //成绩为空
        }
        else
        {
            return sum / count;
        }
    }
}

/**
 * @brief sSql::lesson_total_count 计算课程的总人数
 * @param lesson_id 课程号
 * @return 总人数，如果查询失败则返回-1
 */
int sSql::lesson_total_count(int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM grade WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error when trying to find leson:" + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else
    {
        int cnt = size(query);
        return cnt;
    }
}

/**
 * @brief sSql::lesson_pass_rate 计算课程的及格率
 * @param lesson_id
 * @return
 */
float sSql::lesson_pass_rate(int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM grade WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error when trying to find leson:" + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1;
    }
    else
    {
        float pass = 0;
        float count = 0;
        while (query.next())
        {
            pass += query.value("grade").toFloat() >= 60 ? 1 : 0;
            count += 1;
        }
        qDebug() << "lesson average score calculation finished";
        if (count == 0)
        {
            return -1;    //成绩为空
        }
        else
        {
            return pass / count;
        }
    }
}

/**
 * @brief sSql::add_student_base
 * @param stud_id
 * @param stu_name
 * @param _class
 * @return
 */
bool sSql::add_student_base(int stud_id, QString stu_name, QString _class)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("REPLACE INTO student_info(stu_name,stud_id,class) VALUES(:stu_name,:stud_id,:class)");
    query.bindValue(":stu_name", stu_name);
    query.bindValue(":stud_id", stud_id);
    query.bindValue(":class", _class);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to add new student." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "a new student added";
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
    new_student = new class add_student; //创建界面对象
    new_student->setWindowTitle("new student");//标题
    new_student->show();
    connect(new_student, SIGNAL(send_new_student_signal(int, QString, QString)), this,
        SLOT(try_add_student(int, QString, QString))); //连接信号和槽函数
    return true;
}

/**
 * @brief sSql::try_add_student 处理信号
 * @param stud_id
 * @param stu_name
 * @param _class
 */
void sSql::try_add_student(int stud_id, QString stu_name, QString _class)
{
    if (add_student_base(stud_id, stu_name, _class))
    {
        emit send_student_added_signal();
    }
}

bool sSql::add_lesson_base(QString lesson_name, int lesson_id, QString year, QString term)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("REPLACE INTO lesson_info(lesson_id,lesson_name,year,term) VALUES(:lesson_id,:lesson_name,:year,:term)");
    query.bindValue(":lesson_id", lesson_id);
    query.bindValue(":lesson_name", lesson_name);
    query.bindValue(":year", year);
    query.bindValue(":term", term);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to add new lesson." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "a new lesson added";
        return true;
    }
}

/**
 * @brief sSql::try_add_lesson 尝试添加课程
 * @param lesson_name
 * @param lesson_id
 * @param year
 * @param term
 */
void sSql::try_add_lesson(QString lesson_name, int lesson_id, QString year, QString term)
{
    if (add_lesson_base(lesson_name, lesson_id, year, term))
    {
        emit send_lesson_added_signal();
    }
}

bool sSql::add_lesson()
{
    class add_lesson *new_lesson;
    new_lesson = new class add_lesson; //创建界面对象
    new_lesson->setWindowTitle("new lesson");//标题
    new_lesson->show();
    connect(new_lesson, SIGNAL(send_new_lesson_signal(QString, int, QString, QString)), this,
        SLOT(try_add_lesson(QString, int, QString, QString))); //连接信号和槽函数
    return true;
}


/**
 * @brief sSql::read_all_student 读取所有的学生信息，在QTreeWidget中显示
 * @param content
 * @return true for success
 */
bool sSql::read_all_student(QTreeWidget *content)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM student_info");
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to query table." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        int col = 4;
        content->setColumnCount(col);
        QStringList header({"姓名", "学号", "班级", "优良程度"});
        content->setHeaderLabels(header);
        while (query.next())
        {
            QString stud_id = query.value("stud_id").toString();
            QString stud_name = query.value("stu_name").toString();
            QString _class = query.value("class").toString();
            QString performance = query.value("performance").toString();
            QTreeWidgetItem *new_top_item = new QTreeWidgetItem(QStringList({stud_name, stud_id, _class, performance}), STUDENT); //创建层
            content->addTopLevelItem(new_top_item);//添加顶层
            QTreeWidgetItem  *sec_child_topic = new QTreeWidgetItem(QStringList{"课程名称", "成绩"}); //创建子节点标题
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            sec_child_topic->setDisabled(true);
            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");
            query1.bindValue(":stud_id", stud_id);
            query1.exec();
            while (query1.next())
            {
                QString lesson_name = query1.value("lesson_name").toString();
                QString grade = query1.value("grade").toString();
                QTreeWidgetItem *sec_child_item = new QTreeWidgetItem(QStringList{lesson_name, grade}, GRADE); //创建子节点
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        return true;
    }
}

/**
 * @brief sSql::read_all_lesson 读取所有的课程信息，并在QTreeWidget中显示
 * @param content
 * @return true for success
 */
bool sSql::read_all_lesson(QTreeWidget *content)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info");
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: Fail to query table." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        int col = 4;
        content->setColumnCount(col);
        QStringList header({"课程名称", "课程号", "学年", "学期"}); //父节点标签
        content->setHeaderLabels(header);
        QTreeWidgetItem *new_top_item;
        while (query.next())
        {
            QString lesson_id = query.value("lesson_id").toString();
            QString lesson_name = query.value("lesson_name").toString();
            QString year = query.value("year").toString();
            QString term = query.value("term").toString();
            new_top_item = new QTreeWidgetItem(QStringList({lesson_name, lesson_id, year, term}), LESSON); //创建层
            content->addTopLevelItem(new_top_item);//添加顶层
            QTreeWidgetItem  *sec_child_topic = new QTreeWidgetItem(QStringList{"姓名", "学号", "成绩"}); //创建子节点标签
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            sec_child_topic->setDisabled(true);
            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE lesson_id = :lesson_id");
            query1.bindValue(":lesson_id", lesson_id);
            query1.exec();
            QTreeWidgetItem *sec_child_item;
            while (query1.next())
            {
                QString stu_name = query1.value("stu_name").toString();
                QString stud_id = query1.value("stud_id").toString();
                QString grade = query1.value("grade").toString();
                sec_child_item = new QTreeWidgetItem(QStringList{stu_name, stud_id, grade}, GRADE); //创建子节点
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        return true;
    }
}

/**
 * @brief sSql::search_for_student 搜索学生，并在QTreeWidget中显示
 * @param stu_name
 * @param widget_to_show
 * @return true for success
 */
bool sSql::search_for_student(QString stu_name, QTreeWidget *widget_to_show)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM student_info WHERE stu_name = :stu_name");//要执行的sql语句
    query.bindValue(":stu_name", stu_name);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to find student." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        int col = 4;
        widget_to_show->setColumnCount(col);
        QStringList header({"姓名", "学号", "班级", "优良程度"});
        widget_to_show->setHeaderLabels(header);
        while (query.next())
        {
            QString stud_id = query.value("stud_id").toString();
            QString stud_name = query.value("stu_name").toString();
            QString _class = query.value("class").toString();
            QString performance = query.value("performance").toString();
            QTreeWidgetItem *new_top_item = new QTreeWidgetItem(QStringList({stud_name, stud_id, _class, performance}), STUDENT); //创建层
            widget_to_show->addTopLevelItem(new_top_item);//添加顶层
            QTreeWidgetItem  *sec_child_topic = new QTreeWidgetItem(QStringList{"课程名称", "成绩"}); //创建子节点标题
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            sec_child_topic->setDisabled(true);
            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");
            query1.bindValue(":stud_id", stud_id);
            query1.exec();
            while (query1.next())
            {
                QString lesson_name = query1.value("lesson_name").toString();
                QString grade = query1.value("grade").toString();
                QTreeWidgetItem *sec_child_item = new QTreeWidgetItem(QStringList{lesson_name, grade}, GRADE); //创建子节点
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        widget_to_show->expandAll();
        return true;
    }
}

/**
 * @brief sSql::search_for_lesson 搜索课程，并在QTreeWidget中显示
 * @param lesson_name
 * @param widget_to_show
 * @return tree for success
 */
bool sSql::search_for_lesson(QString lesson_name, QTreeWidget *widget_to_show)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info WHERE lesson_name = :lesson_name");//要执行的sql语句
    query.bindValue(":lesson_name", lesson_name);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to find student. " + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        int col = 4;
        widget_to_show->setColumnCount(col);
        QStringList header({"课程名称", "课程号", "学年", "学期"}); //父节点标签
        widget_to_show->setHeaderLabels(header);
        while (query.next())
        {
            QString lesson_id = query.value("lesson_id").toString();
            QString lesson_name = query.value("lesson_name").toString();
            QString year = query.value("year").toString();
            QString term = query.value("term").toString();
            QTreeWidgetItem *new_top_item = new QTreeWidgetItem(QStringList({lesson_name, lesson_id, year, term}), LESSON); //创建层
            widget_to_show->addTopLevelItem(new_top_item);//添加顶层
            QTreeWidgetItem  *sec_child_topic = new QTreeWidgetItem(QStringList{"姓名", "学号", "成绩"}); //创建子节点标签
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            sec_child_topic->setDisabled(true);
            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE lesson_id = :lesson_id");
            query1.bindValue(":lesson_id", lesson_id);
            query1.exec();
            while (query1.next())
            {
                QString stu_name = query1.value("stu_name").toString();
                QString stud_id = query1.value("stud_id").toString();
                QString grade = query1.value("grade").toString();
                QTreeWidgetItem *sec_child_item = new QTreeWidgetItem(QStringList{stu_name, stud_id, grade}, GRADE); //创建子节点
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        widget_to_show->expandAll();
        return true;
    }
}



/**
 * @brief sSql::add_grade 唤起一个add_grade页面来创建成绩信息
 * @return
 */
bool sSql::add_grade()
{
    class add_score *new_score;
    new_score = new class add_score; //创建界面对象
    new_score->setWindowTitle("new grade");//标题
    new_score->show();
    connect(new_score, SIGNAL(send_new_score_signal(QString, QString, QString, QString, float)), this,
        SLOT(try_add_grade(QString, QString, QString, QString, float))); //连接信号和槽函数
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
void sSql::try_add_grade(QString stu_name, QString lesson_name, QString year, QString term, float grade)
{
    int stud_id = find_student(stu_name);
    int lesson_id = find_lesson(lesson_name, year, term);
    if (add_grade_base(stud_id, lesson_id, stu_name, lesson_name, grade))
    {
        emit send_grade_added_signal();
    }
    calc_performance(stud_id);
}
/**
 * @brief sSql::add_garde_base 向数据库中添加成绩
 * @param stu_id
 * @param lesson_id
 * @param grade
 * @return
 */
bool sSql::add_grade_base(int stu_id, int lesson_id, QString stu_name, QString lesson_name, float grade)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("REPLACE INTO grade (stud_id,lesson_id,stu_name,lesson_name,grade) VALUES (:stud_id,:lesson_id,:stu_name,:lesson_name,:grade)");
    query.bindValue(":stud_id", stu_id);
    query.bindValue(":lesson_id", lesson_id);
    query.bindValue(":stu_name", stu_name);
    query.bindValue(":lesson_name", lesson_name);
    query.bindValue(":grade", grade);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to add new score. " + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "new score added!";
        return true;
    }
}

/**
 * @brief sSql::check_passwd 确认账号密码是否正确
 * @param user_name
 * @param passwd
 * @return true for correct
 */
bool sSql::check_passwd(QString user_name, QString passwd)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM users WHERE user_id = :user_id");
    query.bindValue(":user_id", user_name);
    query.exec();
    if (!query.first()) //没有找到user
    {
        QMessageBox::critical(nullptr, QObject::tr("error"),
            QObject::tr("user not found\n"
                "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else if (passwd == query.value("passwd").toString()) //找到user且密码正确
    {
        qDebug() << "login success!";
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
 * @return true for success
 */
bool sSql::delete_student(int stud_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    //删除该学生的信息
    query.prepare("DELETE FROM student_info WHERE stud_id = :stud_id");//要执行的sql语句
    query.bindValue(":stud_id", stud_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to delete student." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "student deleted!";
        return true;
    }
}

/**
 * @brief sSql::delete_student 重载，删除姓名为stu_name的学生
 * @param stu_name 姓名
 * @return true for success
 */
bool sSql::delete_student(QString stu_name)
{
    int stud_id = find_student(stu_name);
    if (stud_id != -1)
    {
        return delete_student(stud_id);
    }
    else
    {
        return false;
    }
}

/**
 * @brief sSql::delete_grade 删除，学号为stud_id的学生，课程ID为lesson_id的成绩
 * @param stud_id 学号
 * @param lesson_id 课程ID
 * @return true for success
 */
bool sSql::delete_grade(int stud_id, int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("DELETE FROM grade WHERE stud_id = :stud_id AND lesson_id = :lesson_id");//要执行的sql语句
    query.bindValue(":stud_id", stud_id);
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to delete grade. " + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "grade deleted!";
        calc_performance(stud_id);
        calc_lesson_summery(lesson_id);
        return true;
    }
}

/**
 * @brief sSql::delete_grade 重载
 * @param stu_name
 * @param lesson_name
 */
bool sSql::delete_grade(QString stu_name, QString lesson_name)
{
    int lesson_id = find_lesson(lesson_name);
    int stud_id = find_student(stu_name);
    if (lesson_id != -1 && stud_id != -1)
    {
        return delete_grade(stud_id, lesson_id);
    }
    else
    {
        return false;
    }
}

/**
 * @brief sSql::delete_lesson 删除ID为lesson_id的课程
 * @param lesson_id
 * @return true for success
 */
bool sSql::delete_lesson(int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("DELETE FROM lesson_info WHERE lesson_id = :lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id", lesson_id);
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to delete lesson." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug() << "lesson deleted!";
        return true;
    }
}

/**
 * @brief sSql::delete_lesson 重载，删除课程
 * @param lesson_name
 * @param year
 * @param term
 * @return true for success
 */
bool sSql::delete_lesson(QString lesson_name, QString year, QString term)
{
    int lesson_id = find_lesson(lesson_name, year, term);
    if (lesson_id != -1) //课程存在
    {
        return delete_lesson(lesson_id);
    }
    else
    {
        return false;
    }
}



/**
 * @brief sSql::try_delete_student
 * @param stud_id
 */
void sSql::try_delete_student(int stud_id)
{
    if (delete_student(stud_id))
    {
        emit send_student_deleted_signal();
    }
}

/**
 * @brief sSql::try_delete_student
 * @param stu_name
 */
void sSql::try_delete_student(QString stu_name)
{
    if (delete_student(stu_name))
    {
        emit send_student_deleted_signal();
    }
}

/**
 * @brief sSql::try_delete_grade
 * @param stud_id
 * @param lesson_id
 */
void sSql::try_delete_grade(int stud_id, int lesson_id)
{
    if (delete_grade(stud_id, lesson_id))
    {
        emit send_grade_deleted_signal();
    }
}

/**
 * @brief sSql::try_delete_grade
 * @param stu_name
 * @param lesson_name
 * @param year
 * @param term
 */
void sSql::try_delete_grade(QString stu_name, QString lesson_name)
{
    if (delete_grade(stu_name, lesson_name))
    {
        emit send_grade_deleted_signal();
    }
}

/**
 * @brief sSql::update 修改学生信息
 * @param stu_name
 * @param stud_id
 * @param _class
 * @return true for sucsess
 */
bool sSql::update(QString stu_name, int stud_id, QString _class)
{
    bool success = add_student_base(stud_id, stu_name, _class);
    calc_performance(stud_id);
    return success;
}

/**
 * @brief sSql::update 修改成绩信息
 * @param stud_id
 * @param lesson_id
 * @param stu_name
 * @param lesson_name
 * @param grade
 * @return true for sucsess
 */
bool sSql::update(int stud_id, int lesson_id, QString stu_name, QString lesson_name, float grade)
{
    bool success = add_grade_base(stud_id, lesson_id, stu_name, lesson_name, grade);
    calc_performance(stud_id);//需要重新计算平均分和优良程度
    calc_lesson_summery(lesson_id);
    return success;
}

/**
 * @brief sSql::update 修改成绩信息（重载）
 * @param stu_name
 * @param lesson_name
 * @param grade
 * @param year
 * @param term
 * @return true for sucsess
 */
bool sSql::update(QString stu_name, QString lesson_name, float grade)
{
    int stud_id = find_student(stu_name);
    int lesson_id = find_lesson(lesson_name);
    bool success = add_grade_base(stud_id, lesson_id, stu_name, lesson_name, grade);
    calc_performance();//需要重新计算平均分和优良程度
    calc_lesson_summery(lesson_id);
    return success;
}

/**
 * @brief sSql::update 修改课程信息
 * @param lesson_id
 * @param lesson_name
 * @param year
 * @param term
 * @return true for sucsess
 */
bool sSql::update(int lesson_id, QString lesson_name, QString year, QString term)
{
    bool success = add_lesson_base(lesson_name, lesson_id, year, term);
    calc_lesson_summery(lesson_id);
    return success;
}

/**
 * @brief sSql::get 从数据库中获取值
 * @param table_name 表名
 * @param target_name 目标变量名
 * @param key_name 主键数据字段名
 * @param key_value 主键数据字段值
 * @return 以QString格式返回目标值,获取失败返回空串
 */
QString sSql::get(QString table_name, QString target_name, QString key_name, QString key_value)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM " + table_name + " WHERE " + key_name + "=" + key_value); //要执行的sql语句
    if (!query.exec())
    {
        QMessageBox::critical(nullptr, QString("sql error"),
            QString("Error: fail to get value." + query.lastError().text() +
                "\nClick Cancel to exit."), QMessageBox::Cancel);
        return "";
    }
    else
    {
        query.first();
        QString target_value = query.value(target_name).toString();
        return target_value;
    }
}

/**
 * @brief sSql::process_line 处理从文本中获得的一行文字
 * @param line
 * @return true for success
 */
bool sSql::process_line(QString line)
{
    //分割单词
    QStringList list;
    QString word = "";
    foreach (QChar ch, line)
    {
        if (ch != ',' && ch != ':' && ch != ';')
        {
            word += ch;    //获取完整的单词
        }
        else
        {
            list += word; //保存单词
            word = "";
        }
    }
    qDebug() << list;
    if (list.size() < 3 || !(list.size() % 2))
    {
        return false;    //格式错误
    }
    //将数据分类
    QString stu_name = list[0];
    int stud_id = list[1].toInt();
    QString _class = list[2];
    QVector<int> lesson_id;
    QStringList lesson_name;
    QVector<float> grades;
    for (int i = 2; i < (list.size() / 2 + 1); i++)
    {
        lesson_id.push_back(list[2 * i - 1].toInt());
        lesson_name += find_lesson(lesson_id.last());
        grades.push_back(list[2 * i].toFloat());
    }
    qDebug() << "stu_name:" << stu_name;
    qDebug() << "stud_id:" << stud_id;
    qDebug() << "class:" << _class;
    qDebug() << "lesson_id:" << lesson_id;
    qDebug() << "grades:" << grades;
    //将数据加入数据库
    add_student_base(stud_id, stu_name, _class);
    for (int i = 0; i < lesson_id.size(); i++) //把成绩录入
    {
        add_grade_base(stud_id, lesson_id[i], stu_name, lesson_name[i], grades[i]);
        calc_lesson_summery(lesson_id[i]);//更新统计结果
    }
    return true;
}
