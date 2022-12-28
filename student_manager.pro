QT       += core gui
QT       += sql
QTPLUGIN += QSQLMYSQL   #QT提供的数据库驱动
INCLUDEPATH += "F:\MySQL\MySQL Server 8.0\include"   #数据库indlude路径
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_score.cpp \
    add_student.cpp \
    lesson_manage_page.cpp \
    login_page.cpp \
    main.cpp \
    mainwindow.cpp \
    ssql.cpp

HEADERS += \
    add_score.h \
    add_student.h \
    lesson_manage_page.h \
    login_page.h \
    mainwindow.h \
    ssql.h

FORMS += \
    add_score.ui \
    add_student.ui \
    lesson_manage_page.ui \
    login_page.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../../Desktop/程序设计课设/student_manager/logo.jpg \
    ../../Desktop/程序设计课设/student_manager/logo.jpg
