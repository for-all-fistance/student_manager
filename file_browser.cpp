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

void file_browser::init()
{
    QStringList nameFilter;
    nameFilter <<"*.txt";

    model=new QFileSystemModel();
    model->setNameFilterDisables(false);
    model->setNameFilters(nameFilter);
    model->setRootPath("C:");

    ui->my_tree->setModel(model);
    ui->my_tree->setRootIndex(model->index("C:"));
    setWindowTitle("file browser");


    show();

}

void file_browser::on_read_btn_clicked()
{
    QModelIndex index = ui->my_tree->currentIndex();
    if (!index.isValid()) //文件路径无效
        return;
    QString path=model->filePath(index);
    if(read(path))
        emit send_file_read_signal();
    close();
}

/**
 * @brief file_browser::read
 * @param path
 * @return
 */
bool file_browser::read(QString path)
{
    if (path.isEmpty())     //如果未选择文件便确认，即返回
        return true;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("utf-8");//设置读取文本的编码格式为UTF-8
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            qDebug()<<line;

            sql_server=new sSql();
            if(!sql_server->process_line(line))
            {

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

