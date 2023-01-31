/******************************************************************************
  * @file    file_browser.h
  * @author  Jialiang Li
  * @brief   对文件进行操作的页面，选中目标文件并读取
  *
*/
#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#include "qfilesystemmodel.h"
#include "ssql.h"
#include <QWidget>

namespace Ui {
class file_browser;
}

class file_browser : public QWidget
{
    Q_OBJECT

public:
    explicit file_browser(QWidget *parent = nullptr);
    ~file_browser();

signals:
    void send_file_read_signal();
private slots:
    void on_read_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::file_browser *ui;
    void init();
    QFileSystemModel *model;
    bool read(QString path);
    sSql *sql_server;
};

#endif // FILE_BROWSER_H
