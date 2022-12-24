#ifndef CLASS_MANAGE_PAGE_H
#define CLASS_MANAGE_PAGE_H

#include <QWidget>

namespace Ui {
class class_manage_page;
}

class class_manage_page : public QWidget
{
    Q_OBJECT

public:
    explicit class_manage_page(QWidget *parent = nullptr);
    ~class_manage_page();

private:
    Ui::class_manage_page *ui;
};

#endif // CLASS_MANAGE_PAGE_H
