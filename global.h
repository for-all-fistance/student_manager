/******************************************************************************
  * @file    global.h
  * @author  Jialiang Li
  * @brief   声明并定义了一些通用全局变量
  *
*/
#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>

enum {STUDENT, LESSON, GRADE};
extern QString CURRENT_USER;
void set_current_user(QString user);

#endif // GLOBAL_H
