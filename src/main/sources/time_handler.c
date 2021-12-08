#include "../headers/time_handler.h"

#include <stdio.h>
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

int get_week_year(const struct tm *tmptr, int *year, int *week) {
    // work with local copy
    struct tm tm = *tmptr;
    // fully populate the yday and wday fields.
    if (mktime(&tm) == -1) {
        return 1;
    }

    // Find day-of-the-week: 0 to 6.
    // Week starts on Monday per ISO 8601
    // 0 <= DayOfTheWeek <= 6, Monday, Tuesday ... Sunday
    int DayOfTheWeek = (tm.tm_wday + (7 - 1)) % 7;

    // Offset the month day to the Monday of the week.
    tm.tm_mday -= DayOfTheWeek;
    // Offset the month day to the mid-week (Thursday) of the week, 3 days later.
    tm.tm_mday += 3;
    // Re-evaluate tm_year and tm_yday  (local time)
    if (mktime(&tm) == -1) {
        return 1;
    }

    *year = tm.tm_year + 1900;
    // Convert yday to week of the year, stating with 1.
    *week = tm.tm_yday / 7 + 1;
    return 0;
}

time_t get_day_time_from_cal_time(int dow, time_t cal_time) {
    struct tm *tm = localtime(&cal_time);
    tm->tm_hour = 12;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    mktime(tm);
    int DaysSinceMonday = (tm->tm_wday + (7 - 1)) % DAYS_IN_WEEK;
    tm->tm_mday += dow - DaysSinceMonday;
    return mktime(tm);
}

time_t get_cal_time_from_day_time(time_t day_time) {
    return get_day_time_from_cal_time(0, day_time);
}

time_t get_cal_time_from_week_and_year(int week, int year) {
    struct tm tm = {.tm_year = year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday + (7 - 1)) % DAYS_IN_WEEK;
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
