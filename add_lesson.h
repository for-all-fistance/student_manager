#ifndef ADD_LESSON_H
#define ADD_LESSON_H

#include <QWidget>

namespace Ui {
class add_lesson;
}

class add_lesson : public QWidget
{
    Q_OBJECT

public:
    explicit add_lesson(QWidget *parent = nullptr);
    ~add_lesson();

signals:
    void send_new_lesson_signal(QString lesson_name,int lesson_id,QString year,QString term);
private slots:
    void on_confirm_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::add_lesson *ui;
};

#endif // ADD_LESSON_H
