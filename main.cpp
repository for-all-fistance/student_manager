/******************************************************************************
  * @file    main.cpp
  * @author  Jialiang Li
  * @brief   main function
  *
*/
#include "student_manage_page.h"
#include "file_browser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    student_manage_page main;
    return a.exec();
}
