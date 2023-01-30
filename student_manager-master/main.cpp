/******************************************************************************
  * @file    main.cpp
  * @author  Jialiang Li
  * @brief   main function
  *
*/
#include "mainwindow.h"
#include "file_browser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow main;
//    file_browser mybrowser;
//    mybrowser.show();
    return a.exec();
}
