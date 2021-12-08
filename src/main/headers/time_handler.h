#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include <time.h>

enum t_type { t_year,
              t_mon,
              t_week,
              t_dom,
              t_dow,
              t_hour,
              t_min };

int get_t_data(time_t time, int t_type);
int get_week_year(const struct tm *tmptr, int *year, int *week);
time_t get_day_time_from_cal_time(int dow, time_t time);
time_t get_cal_time_from_day_time(time_t day_time);
time_t get_cal_time_from_week_and_year(int week, int year);

#endif
