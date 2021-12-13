#include "../headers/time_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"

static int get_year(struct tm *tm);
static int get_week(struct tm *tm);

/**
 * @brief  Returns given value from time_t
 * @note
 * @param  time: defines the time.
 * @param  t_type:
 * @retval data
 */

int get_t_data(time_t time, int t_type) {
    struct tm *tm = localtime(&time);
    int data;

    switch (t_type) {
        case t_year:
            data = get_year(tm);
            break;
        case t_yday:
            data = tm->tm_yday;
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

/**
 * @brief  Returns and converts time values, along with that controls whether its summer or winter time.
 * @note   if tm_isdst is set to : 0, it means that its basicly always summer and winter time for a correct convertion of time.
 * @param  dow: is a abbreviation for (Day of week).
 * @param  cal_time: calendar time control
 * @retval tm
 */
time_t get_day_time_from_cal_time(int dow, time_t cal_time) {
    struct tm *tm = localtime(&cal_time);
    tm->tm_hour = 12;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = 0;
    mktime(tm);
    int DaysSinceMonday = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday += dow - DaysSinceMonday;
    return mktime(tm);
}
/**
 * @brief  Gets the calendar time from day time.
 * @note   returns day_time.
 * @param  day_time:
 * @retval day_time
 */
time_t get_cal_time_from_day_time(time_t day_time) {
    return get_day_time_from_cal_time(0, day_time);
}

/**
 * @brief  allows you to put a calendar in a year. caluclated how many seconds has gone from 1970 for a correct time convertion.
 * @note   This is a measure used for converting the time depending on diffrent timezones.
 * @param  week: Week
 * @param  year: Year
 * @retval tm
 */
time_t get_cal_time_from_week_and_year(int week, int year) {
    struct tm tm = {.tm_year = year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm.tm_mday += (week - 1) * DAYS_IN_WEEK - DaysSinceMonday;
    return mktime(&tm);
}

/**
 * @brief Rounds time to nearest half hour
 * @note
 * @param  time: Time
 * @param  hour: Hours
 * @param  mins: Minutes
 * @retval tm
 */
time_t digi_time_to_time_t(time_t time, int hour, int mins) {
    if (mins >= 45) {
        mins = 0;
        hour++;
    } else if (mins >= 15) {
        mins = 30;
    } else {
        mins = 0;
    }

    struct tm *tm = localtime(&time);
    tm->tm_hour = hour;
    tm->tm_min = mins;
    return mktime(tm);
}

/**
 * @brief  defines the days of the week.
 * @note
 * @param  *str: Defines a string
 * @param  dow: day of week
 * @retval None
 */
void load_dow_string(char *str, int dow) {
    if (dow == 0) {
        sprintf(str, "Mon");
    } else if (dow == 1) {
        sprintf(str, "Tue");
    } else if (dow == 2) {
        sprintf(str, "Wed");
    } else if (dow == 3) {
        sprintf(str, "Thu");
    } else if (dow == 4) {
        sprintf(str, "Fri");
    } else if (dow == 5) {
        sprintf(str, "Sat");
    } else if (dow == 6) {
        sprintf(str, "Sun");
    }
}

/**
 * @brief  Function for getting a real time value in the value
 * @note
 * @param  cal_time: Calendar time
 * @param  current_time: Current_time
 * @retval cal_time - current_cal_time;
}
 */
int calc_in_week(time_t cal_time, time_t current_time) {
    time_t current_cal_time = get_cal_time_from_day_time(current_time);
    return cal_time - current_cal_time;
}

/**
 * @brief  returns true year of given time.
 * @note
 * @param  *tm:
 * @retval tm->tm_year + 1900;
 */
static int get_year(struct tm *tm) {
    mktime(tm);
    int DayOfTheWeek = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday -= DayOfTheWeek;
    tm->tm_mday += 3;
    mktime(tm);
    return tm->tm_year + 1900;
}

/**
 * @brief  returns true week of given time.
 * @note
 * @param  *tm:
 * @retval tm->tm_yday / DAYS_IN_WEEK + 1;
 */
static int get_week(struct tm *tm) {
    mktime(tm);
    int DayOfTheWeek = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday -= DayOfTheWeek;
    tm->tm_mday += 3;
    mktime(tm);
    return tm->tm_yday / DAYS_IN_WEEK + 1;
}
