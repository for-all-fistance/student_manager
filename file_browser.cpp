/******************************************************************************
  * @file    file_browser.cpp
  * @author  Jialiang Li
  * @brief   file_browser类的具体实现
  *
*/
#pragma execution_character_set("utf-8")

#include "qdebug.h"
#include "file_browser.h"
#include "ui_file_browser.h"

file_browser::file_browser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file_browser)
{
    ui->setupUi(this);
    init();
}

file_browser::~file_browser()
{
    delete ui;
}

/**
 * @brief file_browser::init 初始化页面，并定位在C盘。
 */
void file_browser::init()
{
    QStringList nameFilter;
    nameFilter << "*.txt";
    model = new QFileSystemModel();
    model->setNameFilterDisables(false);
    model->setNameFilters(nameFilter);
    model->setRootPath("C:");
    //model->setRootPath(QDir::currentPath());
    ui->my_tree->setModel(model);
    ui->my_tree->setRootIndex(model->index("C:"));
    //ui->my_tree->setRootIndex(model->index(QDir::currentPath()));
    setWindowTitle("file browser");
    show();
}

/**
 * @brief file_browser::on_read_btn_clicked 点击确认按钮，开始读取目标文件
 */
void file_browser::on_read_btn_clicked()
{
    QModelIndex index = ui->my_tree->currentIndex();
    if (!index.isValid()) //文件路径无效
    {
        return;
    }
    QString path = model->filePath(index);
    if (read(path))
    {
        emit send_file_read_signal();
    }
    close();
}

/**
 * @brief file_browser::read 读取目标文件，并交给数据库保存
 * @param path
 * @return
 */
bool file_browser::read(QString path)
{
    if (path.isEmpty())     //如果未选择文件便确认，即返回
    {
        return false;
    }
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        sql_server = new sSql();
        QTextStream stream(&file);
        stream.setCodec("utf-8");//设置读取文本的编码格式为UTF-8
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            if (!sql_server->process_line(line))
            {
                QMessageBox::critical(this, QObject::tr("snytex error"),
                    "格式错误："
                    + line +
                    "\nClick Cancel to exit.", QMessageBox::Cancel);
            }
        }
        file.close();
    }
    return true;
}


void file_browser::on_cancel_btn_clicked()
{
    close();
}

