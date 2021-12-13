#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

/************************************************************/

#include <time.h>

/************************************************************/

enum t_type {
    t_year,
    t_yday,
    t_mon,
    t_week,
    t_dom,
    t_dow,
    t_hour,
    t_min,
};

/************************************************************/

int get_t_data(time_t time, int t_type);
time_t get_day_time_from_cal_time(int dow, time_t time);
time_t get_cal_time_from_day_time(time_t day_time);
time_t get_cal_time_from_week_and_year(int week, int year);
time_t digi_time_to_time_t(time_t time, int hour, int mins);
void load_dow_string(char *str, int dow);
int calc_in_week(time_t cal_time, time_t current_time);

/************************************************************/

#endif
