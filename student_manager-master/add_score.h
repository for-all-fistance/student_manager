#ifndef ADD_SCORE_H
#define ADD_SCORE_H

#include <QWidget>

namespace Ui {
class add_score;
}

class add_score : public QWidget
{
    Q_OBJECT

public:
    explicit add_score(QWidget *parent = nullptr);
    ~add_score();

private slots:
    void on_new_score_btn_clicked();

    void on_cancel_clicked();

private:
    Ui::add_score *ui;
signals:
    void send_new_score_signal(QString stu_name,QString lesson_name,QString year,QString term,float grade);
};

#endif // ADD_SCORE_H
