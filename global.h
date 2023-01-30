#ifndef GLOBAL_H
#define GLOBAL_H

enum{STUDENT,LESSON,GRADE};

struct lesson_summery
{
    lesson_summery() {}
    lesson_summery(int _total_cnt,float _pass_rt,float avrg_score):
    total_cnt(_total_cnt),pass_rate(_pass_rt),average_score(avrg_score){}
    int total_cnt;
    float pass_rate;
    float average_score;
};

#endif // GLOBAL_H
