 /******************************************************************************
  * @file    ssql.cpp
  * @author  Jialiang Li
  * @brief   member function of class ssql,including all the sql operations, such as  add, search...
  *
*/
#pragma execution_character_set("utf-8")

#include "ssql.h"
#include "global.h"

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
    query.prepare(find);
    query.bindValue(":stud_id", stud_id);//使用 bindValue函数将占位符替换
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find student. " << query.lastError();
        return "";
    }
    else if(query.first())
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
    query.prepare("SELECT * FROM student_info WHERE stu_name=:stu_name");
    query.bindValue(":stu_name",stu_name);//使用 bindValue函数将占位符替换
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find student. " << query.lastError();
        return -1;
    }
    else if(query.first())
    {

        int stud_id=query.value("stud_id").toInt();
        qDebug() << "student found!";
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
    query.prepare("SELECT * FROM lesson_info WHERE lesson_id=:lesson_id");
    query.bindValue(":lesson_id", lesson_id);//使用 bindValue函数将占位符替换
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find lesson. " << query.lastError();
        return "";
    }
    else if(query.first())
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
    query.prepare("SELECT * FROM lesson_info WHERE lesson_name = :lesson_name AND year = :year AND term = :term");
    query.bindValue(":lesson_name", lesson_name);//使用 bindValue函数将占位符替换
    query.bindValue(":year",year);
    query.bindValue(":term",term);
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find lesson. " << query.lastError();
        return -1;
    }
    else if(query.first())
    {     qDebug() << "lesson found!";
        int stu_id=query.value("lesson_id").toInt();
        return stu_id;
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
    if(!query.exec())// 执行sql语句
    {
        qDebug() << "Error: Fail to find lesson. " << query.lastError();
        return -1;
    }
    else if(query.first())
    {     qDebug() << "lesson found!";
        int lesson_id=query.value("lesson_id").toInt();
        return lesson_id;
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
    if(!query.exec())
    {
        qDebug() << "Error: fail to find student when calculating perfoemance. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"ready to calculate performance.";
        while (query.next()) {
                calc_performance(query.value("stud_id").toInt());
        }
        qDebug()<<"calculation finished!";
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
        float avrg=student_average(stud_id);
        QSqlQuery query(mydb);// 用于执行sql语句的对象
        query.prepare("UPDATE student_info SET performance=:performance,average_score=:average_score WHERE stud_id = :stud_id");//要执行的sql语句
        query.bindValue(":stud_id",stud_id);
        if(avrg>=90) query.bindValue(":performance","优");
        else if(avrg>=80)   query.bindValue(":performance","良");
        else if(avrg>=60)   query.bindValue(":performance","合格");
        else query.bindValue(":performance","不合格");
        query.bindValue(":average_score",avrg);//更新成绩
        if(!query.exec())
        {
            qDebug() << "Error: fail to calculate performance of student "+QString(stud_id) << query.lastError();
            return false;
        }
        else
        {
            //query.first();
            qDebug()<<QString(stud_id);
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
    query.bindValue(":stud_id",stud_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to calculate average score. " << query.lastError();
        return -1;
    }
    else
    {
        qDebug()<<"ready to calculate average score.";
        float sum=0;
        int count=0;
        while (query.next()) {
            sum+=query.value("grade").toFloat();
            count+=1;
        }
        if(count==0)    return -1;//成绩为空
        else    return sum/count;
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
    if(!query.exec())
    {
        qDebug() << "Error when trying to calculate lesson summery: . " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"ready to calculate lesson summery.";
        while (query.next()) {
            calc_lesson_summery(query.value("lesson_id").toInt());
        }
        qDebug()<<"calculation finished";
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
    float pass_rt=0;
    float avrg_score=0;
    int perfect=0;
    int good=0;
    int fail=0;
    int qualified=0;
    int total_cnt=0;
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM grade WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error when trying to find leson:  " << query.lastError();
        return -1;
    }
    else
    {
        int pass=0;
        float sum=0;
        int count=0;
        while (query.next()) {
            float grade=query.value("grade").toFloat();
            sum+=grade;
            pass+=grade>=60?1:0;
            perfect+=grade>=90?1:0;
            good+=grade>=80&&grade<90?1:0;
            qualified+=grade>=60&&grade<80?1:0;
            fail+=grade<60?1:0;
            count+=1;
        }
        if(count!=0)    //成绩不为空
        {
            pass_rt=float(pass)/count;
            avrg_score=sum/count;
            total_cnt=count;
        }
    }
    query.prepare("UPDATE lesson_info SET total_count=:total_cnt,average_score=:avrg_score,pass_rate=:pass_rt,"
                  "perfect=:perfect,good=:good,fail=:fail,qualified=:qualified "
                  "WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":total_cnt",total_cnt);
    query.bindValue(":avrg_score",avrg_score);
    query.bindValue(":pass_rt",pass_rt*100);
    query.bindValue(":perfect",perfect);
    query.bindValue(":good",good);
    query.bindValue(":fail",fail);
    query.bindValue(":qualified",qualified);
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error: in sSql::calc_lesson_summery(int lesson_id). " << query.lastError();
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
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error when trying to find leson:  " << query.lastError();
        return -1;
    }
    else
    {
        float sum=0;
        int count=0;
        while (query.next()) {
            sum+=query.value("grade").toFloat();
            count+=1;
        }
        qDebug()<<"lesson average score calculation finished";
        if(count==0)    return -1;//成绩为空
        else    return sum/count;
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
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error when trying to find leson:  " << query.lastError();
        return -1;
    }
    else
    {
        int cnt=size(query);
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
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error when trying to find leson:  " << query.lastError();
        return -1;
    }
    else
    {
        float pass=0;
        float count=0;
        while (query.next()) {
            pass+=query.value("grade").toFloat()>=60?1:0;
            count+=1;
        }
        qDebug()<<"lesson average score calculation finished";
        if(count==0)    return -1;//成绩为空
        else    return pass/count;
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
    query.prepare("INSERT INTO student_info(stu_name,stud_id,class) VALUES(:stu_name,:stud_id,:class)");
    query.bindValue(":stu_name", stu_name);
    query.bindValue(":stud_id", stud_id);
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

bool sSql::add_lesson_base(QString lesson_name,int lesson_id,QString year,QString term)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("INSERT INTO lesson_info(lesson_id,lesson_name,year,term) VALUES(:lesson_id,:lesson_name,:year,:term)");
    query.bindValue(":lesson_id", lesson_id);
    query.bindValue(":lesson_name", lesson_name);
    query.bindValue(":year", year);
    query.bindValue(":term", term);
    if(!query.exec())
    {
        qDebug() << "Error: Fail to add new lesson. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"a new lesson added";
        return true;
    }
}

void sSql::try_add_lesson(QString lesson_name,int lesson_id,QString year,QString term)
{
    if(add_lesson_base(lesson_name,lesson_id,year,term))
        emit send_lesson_added_signal();
}

bool sSql::add_lesson()
{
    class add_lesson *new_lesson;
    new_lesson=new class add_lesson;//创建界面对象
    new_lesson->setWindowTitle("new lesson");//标题
    new_lesson->show();
    connect(new_lesson,SIGNAL(send_new_lesson_signal(QString,int,QString,QString)),this,
            SLOT(try_add_lesson(QString,int,QString,QString)));//连接信号和槽函数
    return true;
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
            //qDebug()<<QString("id:%1    name:%2    class:%3   performance:%4").arg(stud_id).arg(stud_name).arg(_class).arg(performance);
            QTreeWidgetItem* new_top_item = new QTreeWidgetItem(QStringList({stud_name,stud_id,_class,performance}),STUDENT);//创建层
            //new_top_item->setCheckState(0, Qt::Unchecked);
            content->addTopLevelItem(new_top_item);//添加顶层

            QTreeWidgetItem*  sec_child_topic= new QTreeWidgetItem(QStringList{"课程名称","成绩"});//创建子节点标题
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            //sec_child_topic->setFlags(sec_child_topic->flags() | Qt::edit)
            sec_child_topic->setDisabled(true);

            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");
            query1.bindValue(":stud_id",stud_id);
            query1.exec();
            while(query1.next())
            {
                QString lesson_name=query1.value("lesson_name").toString();
                QString grade=query1.value("grade").toString();
                QTreeWidgetItem* sec_child_item = new QTreeWidgetItem(QStringList{lesson_name,grade},GRADE);//创建子节点
                //sec_child_item->setCheckState(0,Qt::Unchecked);
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上       
            }
        }
        return true;
    }
}

/**
 * @brief sSql::read_all_lesson
 * @param content
 * @return
 */
bool sSql::read_all_lesson(QTreeWidget *content)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info");
    if(!query.exec())
    {
        qDebug() << "Error: Fail to query table. " << query.lastError();
        return false;
    }
    else
    {
        int col=4;
        content->setColumnCount(col);
        QStringList header({"课程名称","课程号","学年","学期"});//父节点标签
        content->setHeaderLabels(header);
        QTreeWidgetItem *new_top_item;
        while(query.next())
        {
            QString lesson_id = query.value("lesson_id").toString();
            QString lesson_name = query.value("lesson_name").toString();
            QString year = query.value("year").toString();
            QString term=query.value("term").toString();
            //qDebug()<<QString("id:%1    name:%2    year:%3   term:%4").arg(lesson_id).arg(lesson_name).arg(year).arg(term);
            new_top_item= new QTreeWidgetItem(QStringList({lesson_name,lesson_id,year,term}),LESSON);//创建层
            //new_top_item->setCheckState(0, Qt::Unchecked);
            content->addTopLevelItem(new_top_item);//添加顶层

            QTreeWidgetItem*  sec_child_topic= new QTreeWidgetItem(QStringList{"姓名","学号","成绩"});//创建子节点标签
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            //sec_child_topic->setFlags(sec_child_topic->flags() | Qt::edit)
            sec_child_topic->setDisabled(true);

            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE lesson_id = :lesson_id");
            query1.bindValue(":lesson_id",lesson_id);
            query1.exec();
            QTreeWidgetItem *sec_child_item;
            while(query1.next())
            {
                QString stu_name=query1.value("stu_name").toString();
                QString stud_id=query1.value("stud_id").toString();
                QString grade=query1.value("grade").toString();
                sec_child_item = new QTreeWidgetItem(QStringList{stu_name,stud_id,grade},GRADE);//创建子节点
                //sec_child_item->setCheckState(0,Qt::Unchecked);
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }

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
        int col=4;
        widget_to_show->setColumnCount(col);
        QStringList header({"姓名","学号","班级","优良程度"});
        widget_to_show->setHeaderLabels(header);
        while(query.next())
        {
            QString stud_id = query.value("stud_id").toString();
            QString stud_name = query.value("stu_name").toString();
            QString _class = query.value("class").toString();
            QString performance=query.value("performance").toString();
            //qDebug()<<QString("id:%1    name:%2    class:%3   performance:%4").arg(stud_id).arg(stud_name).arg(_class).arg(performance);
            QTreeWidgetItem* new_top_item = new QTreeWidgetItem(QStringList({stud_name,stud_id,_class,performance}),STUDENT);//创建层
            //new_top_item->setCheckState(0, Qt::Unchecked);
            widget_to_show->addTopLevelItem(new_top_item);//添加顶层

            QTreeWidgetItem*  sec_child_topic= new QTreeWidgetItem(QStringList{"课程名称","成绩"});//创建子节点标题
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            //sec_child_topic->setFlags(sec_child_topic->flags() | Qt::edit)
            sec_child_topic->setDisabled(true);

            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE stud_id = :stud_id");
            query1.bindValue(":stud_id",stud_id);
            query1.exec();
            while(query1.next())
            {
                QString lesson_name=query1.value("lesson_name").toString();
                QString grade=query1.value("grade").toString();
                QTreeWidgetItem* sec_child_item = new QTreeWidgetItem(QStringList{lesson_name,grade},GRADE);//创建子节点
                //sec_child_item->setCheckState(0,Qt::Unchecked);
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
        }
        return true;
    }
}

/**
 * @brief sSql::search_for_lesson
 * @param lesson_name
 * @param widget_to_show
 * @return
 */
bool sSql::search_for_lesson(QString lesson_name,QTreeWidget *widget_to_show)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM lesson_info WHERE lesson_name = :lesson_name");//要执行的sql语句
    query.bindValue(":lesson_name",lesson_name);
    if(!query.exec())
    {
        qDebug() << "Error: fail to find student. " << query.lastError();
        return false;
    }
    else
    {
        int col=4;
        widget_to_show->setColumnCount(col);
        QStringList header({"课程名称","课程号","学年","学期"});//父节点标签
        widget_to_show->setHeaderLabels(header);
        while(query.next())
        {
            QString lesson_id = query.value("lesson_id").toString();
            QString lesson_name = query.value("lesson_name").toString();
            QString year = query.value("year").toString();
            QString term=query.value("term").toString();
            //qDebug()<<QString("id:%1    name:%2    year:%3   term:%4").arg(lesson_id).arg(lesson_name).arg(year).arg(term);
            QTreeWidgetItem* new_top_item = new QTreeWidgetItem(QStringList({lesson_name,lesson_id,year,term}),LESSON);//创建层
            //new_top_item->setCheckState(0, Qt::Unchecked);
            widget_to_show->addTopLevelItem(new_top_item);//添加顶层

            QTreeWidgetItem*  sec_child_topic= new QTreeWidgetItem(QStringList{"姓名","学号","成绩"});//创建子节点标签
            new_top_item->addChild(sec_child_topic);    //将子节点标题添加到顶层节点上
            //sec_child_topic->setFlags(sec_child_topic->flags() | Qt::edit)
            sec_child_topic->setDisabled(true);

            QSqlQuery query1(mydb);//开始创建带有学生成绩的子节点
            query1.prepare("SELECT * FROM grade WHERE lesson_id = :lesson_id");
            query1.bindValue(":lesson_id",lesson_id);
            query1.exec();
            while(query1.next())
            {
                QString stu_name=query1.value("stu_name").toString();
                QString stud_id=query1.value("stud_id").toString();
                QString grade=query1.value("grade").toString();
                QTreeWidgetItem* sec_child_item = new QTreeWidgetItem(QStringList{stu_name,stud_id,grade},GRADE);//创建子节点
                //sec_child_item->setCheckState(0,Qt::Unchecked);
                new_top_item->addChild(sec_child_item);    //将子节点添加到顶层节点上
            }
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
    int stud_id=find_student(stu_name);
    int lesson_id=find_lesson(lesson_name,year,term);
    if(add_grade_base(stud_id,lesson_id,stu_name,lesson_name,grade))
        emit send_grade_added_signal();
    calc_performance(stud_id);
}
/**
 * @brief sSql::add_garde_base
 * @param stu_id
 * @param lesson_id
 * @param grade
 * @return
 */
bool sSql::add_grade_base(int stu_id,int lesson_id,QString stu_name,QString lesson_name,float grade)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("INSERT INTO grade (stud_id,lesson_id,stu_name,lesson_name,grade) VALUES (:stud_id,:lesson_id,:stu_name,:lesson_name,:grade)");
    query.bindValue(":stud_id",stu_id);
    query.bindValue(":lesson_id",lesson_id);
    query.bindValue(":stu_name",stu_name);
    query.bindValue(":lesson_name",lesson_name);
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
bool sSql::delete_student(int stud_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    //删除该学生的信息
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
}

/**
 * @brief sSql::delete_student 重载，删除姓名为stu_name的学生
 * @param stu_name 姓名
 * @return
 */
bool sSql::delete_student(QString stu_name)
{
    int stud_id=find_student(stu_name);
    if(stud_id!=-1)
        return delete_student(stud_id);
    else
        return false;
}

/**
 * @brief sSql::delete_grade 删除，学号为stud_id的学生，课程ID为lesson_id的成绩
 * @param stud_id 学号
 * @param lesson_id 课程ID
 * @return
 */
bool sSql::delete_grade(int stud_id,int lesson_id)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("DELETE FROM grade WHERE stud_id = :stud_id AND lesson_id = :lesson_id");//要执行的sql语句
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
        calc_performance(stud_id);
        calc_lesson_summery(lesson_id);
        return true;
    }
}

/**
 * @brief sSql::delete_grade 重载
 * @param stu_name
 * @param lesson_name
 * @param year
 * @param term
 * @return
 */
bool sSql::delete_grade(QString stu_name,QString lesson_name,QString year,QString term)
{
    int lesson_id=find_lesson(lesson_name,year,term);
    int stud_id=find_student(stu_name);
    if(lesson_id!=-1&&stud_id!=-1)
        return delete_grade(stud_id,lesson_id);
    else
        return false;
}

/**
 * @brief sSql::delete_lesson 删除ID为lesson_id的课程
 * @param lesson_id
 * @return true if success
 */
bool sSql::delete_lesson(int lesson_id)
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
bool sSql::delete_lesson(QString lesson_name,QString year,QString term)
{
    int lesson_id=find_lesson(lesson_name,year,term);
    if(lesson_id!=-1)//课程存在
        return delete_lesson(lesson_id);
    else
        return false;
}



/**
 * @brief sSql::try_delete_student
 * @param stud_id
 */
void sSql::try_delete_student(int stud_id)
{
    if(delete_student(stud_id))
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
    if(delete_student(stu_name))
    {
        emit send_student_deleted_signal();
    }
}

/**
 * @brief sSql::try_delete_grade
 * @param stud_id
 * @param lesson_id
 */
void sSql::try_delete_grade(int stud_id,int lesson_id)
{
    if(delete_grade(stud_id,lesson_id))
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
    if(delete_grade(stu_name,lesson_name,year,term))
        emit send_grade_deleted_signal();
}

/**
 * @brief sSql::update 修改学生信息
 * @param stu_name
 * @param stud_id
 * @param _class
 * @param performance
 * @return
 */
bool sSql::update(QString stu_name,int stud_id,QString _class)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("UPDATE student_info SET stu_name=:stu_name,class=:class WHERE stud_id=:stud_id");//要执行的sql语句
    query.bindValue(":stu_name",stu_name);
    query.bindValue(":class",_class);
    query.bindValue(":stud_id",stud_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to update. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"update success!";
        return true;
    }
}

/**
 * @brief sSql::update 修改成绩信息
 * @param stud_id
 * @param lesson_id
 * @param stu_name
 * @param lesson_name
 * @param grade
 * @return
 */
bool sSql::update(int stud_id,int lesson_id,QString stu_name,QString lesson_name,float grade)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("UPDATE grade SET stu_name=:stu_name,lesson_name=:lesson_name,grade=:grade WHERE stud_id=:stud_id AND lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":stu_name",stu_name);
    query.bindValue(":lesson_name",lesson_name);
    query.bindValue(":grade",grade);
    query.bindValue(":stud_id",stud_id);
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to update. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"update success!";
        calc_performance(stud_id);//需要重新计算平均分和优良程度
        calc_lesson_summery(lesson_id);
        return true;
    }
}

/**
 * @brief sSql::update 修改成绩信息（重载）
 * @param stu_name
 * @param lesson_name
 * @param grade
 * @param year
 * @param term
 * @return
 */
bool sSql::update(QString stu_name,QString lesson_name,float grade)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("UPDATE grade SET stu_name=:stu_name,grade=:grade,lesson_name=:lesson_name WHERE stud_id=:stud_id AND lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":stu_name",stu_name);
    query.bindValue(":lesson_name",lesson_name);
    query.bindValue(":grade",grade);

    //为确保修改后的数据正常，需要保证修改后的学生和课程已经存在
    int stud_id=find_student(stu_name);
    if(stud_id==-1)
    {
        QMessageBox::critical(nullptr, QObject::tr("无效"),
                              QObject::tr("学生不存在，请检查后重新修改！"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    query.bindValue(":stud_id",stud_id);



    int lesson_id=find_lesson(lesson_name);
    if(lesson_id==-1)
    {
        QMessageBox::critical(nullptr, QObject::tr("无效"),
                              QObject::tr("课程不存在，请检查后重新修改！"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }
    query.bindValue(":lesson_id",lesson_id);

    if(!query.exec())
    {
        qDebug() << "Error: fail to update. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"update success!";
        calc_performance();//需要重新计算平均分和优良程度
        calc_lesson_summery(lesson_id);
        return true;
    }
}

/**
 * @brief sSql::update 修改课程信息
 * @param lesson_id
 * @param lesson_name
 * @param year
 * @param term
 * @return
 */
bool sSql::update(int lesson_id,QString lesson_name,QString year,QString term)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("UPDATE lesson_info SET lesson_name=:lesson_name,year=:year,term=:term WHERE lesson_id=:lesson_id");//要执行的sql语句
    query.bindValue(":lesson_name",lesson_name);
    query.bindValue(":year",year);
    query.bindValue(":term",term);
    query.bindValue(":lesson_id",lesson_id);
    if(!query.exec())
    {
        qDebug() << "Error: fail to update. " << query.lastError();
        return false;
    }
    else
    {
        qDebug()<<"update success!";
        return true;
    }
}

/**
 * @brief sSql::get 从数据库中获取值
 * @param table_name 表名
 * @param target_name 目标变量名
 * @param key_name 主键数据字段名
 * @param key_value 主键数据字段值
 * @return 以QString格式返回目标值
 */
QString sSql::get(QString table_name,QString target_name,QString key_name,QString key_value)
{
    QSqlQuery query(mydb);// 用于执行sql语句的对象
    query.prepare("SELECT * FROM "+table_name+" WHERE "+key_name+"="+key_value);//要执行的sql语句
    if(!query.exec())
    {
        qDebug() << "Error: fail to get value. " << query.lastError();
        return "";
    }
    else
    {
        query.first();
        QString target_value=query.value(target_name).toString();
        qDebug()<<"get value "+target_name;
        return target_value;
    }
}

/**
 * @brief sSql::process_line 处理从文本中获得的一行文字
 * @param line
 * @return
 */
bool sSql::process_line(QString line)
{
    //分割单词
    QStringList list;
    QString word;
    foreach (QChar ch, line)
    {
        if(ch!=',' && ch!=':' && ch!=';')
            word+=ch;//获取完整的单词
        else
        {
            list+=word;//保存单词
            word="";
        }
    }
    qDebug()<<list;

    //将数据分类
    QString stu_name=list[0];
    int stud_id=list[1].toInt();
    QString _class=list[2];
    QVector<int> lesson_id;
    QStringList lesson_name;
    QVector<float> grades;
    for(int i=2;i<(list.size()/2+1);i++)
    {
        lesson_id.push_back(list[2*i-1].toInt());
        lesson_name+=find_lesson(lesson_id.last());
        grades.push_back(list[2*i].toFloat());
    }
    qDebug()<<"stu_name:"<<stu_name;
    qDebug()<<"stud_id:"<<stud_id;
    qDebug()<<"class:"<<_class;
    qDebug()<<"lesson_id:"<<lesson_id;
    qDebug()<<"grades:"<<grades;

    //将数据加入数据库
    if(update(stu_name,stud_id,_class))//如果已经学生存在就修改其信息，不存在就新建
    {
            for(int i=0;i<lesson_id.size();i++)//把成绩录入
            {
                if(update(stud_id,lesson_id[i],stu_name,lesson_name[i],grades[i]))
                    add_grade_base(stud_id,lesson_id[i],stu_name,lesson_name[i],grades[i]);
            }
    }
    else
    {
        for(int i=0;i<lesson_id.size();i++)//把成绩录入
            add_grade_base(stud_id,lesson_id[i],stu_name,lesson_name[i],grades[i]);
    }
    return true;
}
