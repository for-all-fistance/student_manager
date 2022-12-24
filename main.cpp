/******************************************************************************
  * @file    main.cpp
  * @author  Jialiang Li
  * @brief   main function
  *
*/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main;
    lesson_manage_page lesson_manage;
    return a.exec();
}
