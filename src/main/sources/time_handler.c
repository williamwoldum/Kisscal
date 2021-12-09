#include "../headers/time_handler.h"

#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"

static int get_year(struct tm *tm);
static int get_week(struct tm *tm);

int get_t_data(time_t time, int t_type) {
    struct tm *tm = localtime(&time);
    int data;

    switch (t_type) {
        case t_year:
            data = get_year(tm);
            break;
        case t_mon:
            data = tm->tm_mon + 1;
            break;
        case t_week:
            data = get_week(tm);
            break;
        case t_dom:
            data = tm->tm_mday;
            break;
        case t_dow:
            data = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
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
    int DaysSinceMonday = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday += dow - DaysSinceMonday;
    return mktime(tm);
}

time_t get_cal_time_from_day_time(time_t day_time) {
    return get_day_time_from_cal_time(0, day_time);
}

time_t get_cal_time_from_week_and_year(int week, int year) {
    struct tm tm = {.tm_year = year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm.tm_mday += (week - 1) * DAYS_IN_WEEK - DaysSinceMonday;
    return mktime(&tm);
}

time_t digi_time_to_time_t(time_t time, int hour, int mins) {
    if (mins >= 39) {
        mins = 0;
        hour++;
    } else if (mins >= 18) {
        mins = 30;
    } else {
        mins = 0;
    }

    struct tm *tm = localtime(&time);
    tm->tm_hour = hour;
    tm->tm_min = mins;
    return mktime(tm);
}

static int get_year(struct tm *tm) {
    mktime(tm);
    int DayOfTheWeek = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday -= DayOfTheWeek;
    tm->tm_mday += 3;
    mktime(tm);
    return tm->tm_year + 1900;
}

static int get_week(struct tm *tm) {
    mktime(tm);
    int DayOfTheWeek = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday -= DayOfTheWeek;
    tm->tm_mday += 3;
    mktime(tm);
    return tm->tm_yday / DAYS_IN_WEEK + 1;
}
