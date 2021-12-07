#include "../headers/time_handler.h"

#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"

int get_t_data(time_t time, int t_type) {
    struct tm *tm = localtime(&time);
    int data;

    switch (t_type) {
        case t_year:
            data = tm->tm_year + 1900;
            break;
        case t_mon:
            data = tm->tm_mon + 1;
            break;
        case t_week: {
            char week_number_str[3];
            strftime(week_number_str, 3, "%W", tm);
            data = atoi(week_number_str);
            break;
        }
        case t_dom:
            data = tm->tm_mday;
            break;
        case t_dow:
            data = tm->tm_wday;
            break;
        case t_hour:
            data = tm->tm_hour;
            break;
        case t_min:
            data = tm->tm_min;
            break;
        default:
            data = -1;
    }
    return data;
}

time_t get_day_time_from_cal_time(int dow, time_t cal_time) {
    struct tm *tm = localtime(&cal_time);
    tm->tm_hour = 12;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    mktime(tm);
    int DaysSinceMonday = (tm->tm_wday - 1) % DAYS_IN_WEEK;
    tm->tm_mday += dow - DaysSinceMonday;
    return mktime(tm);
}

time_t get_cal_time_from_day_time(time_t day_time) {
    return get_day_time_from_cal_time(0, day_time);
}

time_t get_cal_time_from_week_and_year(int week, int year) {
    struct tm tm = {.tm_year = year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday - 1) % DAYS_IN_WEEK;
    tm.tm_mday += (week - 1) * DAYS_IN_WEEK - DaysSinceMonday;
    return mktime(&tm);
}

void constrain_week(int *week, int *year, time_t *cal_time) {
    /* if (*year < 1900) {
        *year = 1900;
    } else if (*year > 9999) {
        *year = 9999;
    }

    struct tm tm = {.tm_year = *year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday - 1) % DAYS_IN_WEEK;
    tm.tm_mday += (*week - 1) * DAYS_IN_WEEK - DaysSinceMonday;
    mktime(&tm);

    *year = tm.tm_year + 1900;
    char week_number_str[3];
    strftime(week_number_str, 3, "%W", &tm);
    *week = atoi(week_number_str); */
}
