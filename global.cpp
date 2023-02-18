/******************************************************************************
  * @file    global.cpp
  * @author  Jialiang Li
  * @brief   定义全局变量和操作函数
  *
*/
#include "global.h"

QString CURRENT_USER = "default";

/**
 * @brief set_current_user 设置当前登录的账号
 * @param user
 */
void set_current_user(QString user)
{
    CURRENT_USER = user;
}
