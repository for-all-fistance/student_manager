#include "class_manage_page.h"
#include "ui_class_manage_page.h"

class_manage_page::class_manage_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::class_manage_page)
{
    ui->setupUi(this);
}

class_manage_page::~class_manage_page()
{
    delete ui;
}
