#include "../headers/time_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"

/************************************************************************* Static function prototypes */

static int get_year(struct tm *tm);
static int get_week(struct tm *tm);

/************************************************************************* Global functions  */

/**
 * @brief  Returns time data from time_t
 * @note
 * @param  time: Time from which the data can be extracted
 * @param  t_type: Indicates type of time data to be extracted
 * @retval int (time data)
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
 * @brief  Returns day time/id of given day in given week
 * @note
 * @param  dow: Index of day in week
 * @param  cal_time: Week time
 * @retval time_t (12:00 am on day, acts as id)
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
 * @brief  Returns calendar time/id extracted from given day time in week
 * @note
 * @param  day_time: 12:00 am on day, acts as id
 * @retval time_t (Monday 12am in week, acts as id)
 */
time_t get_cal_time_from_day_time(time_t day_time) {
    return get_day_time_from_cal_time(0, day_time);
}

/**
 * @brief  Returns calendar time/id from given week and year
 * @note
 * @param  week: Week of calendar
 * @param  year: Year of calendar
 * @retval time_t (Monday 12am in week, acts as id)
 */
time_t get_cal_time_from_week_and_year(int week, int year) {
    struct tm tm = {.tm_year = year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm.tm_mday += (week - 1) * DAYS_IN_WEEK - DaysSinceMonday;
    return mktime(&tm);
}

/**
 * @brief Rounds time_t to nearest half hour
 * @note
 * @param  time: Time to be rounded
 * @param  hour: Current hour of time
 * @param  mins: Current minutes of time
 * @retval time_t
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
 * @brief  Loads day string from day index
 * @note
 * @param  *buf: Buffer to load into
 * @param  dow: Index of day in week
 * @retval None
 */
void load_dow_string(char *buf, int dow) {
    if (dow == 0) {
        sprintf(buf, "Mon");
    } else if (dow == 1) {
        sprintf(buf, "Tue");
    } else if (dow == 2) {
        sprintf(buf, "Wed");
    } else if (dow == 3) {
        sprintf(buf, "Thu");
    } else if (dow == 4) {
        sprintf(buf, "Fri");
    } else if (dow == 5) {
        sprintf(buf, "Sat");
    } else if (dow == 6) {
        sprintf(buf, "Sun");
    }
}

/**
 * @brief  Checks if time is in real world week, negative if the cal is before the current week, positive after, 0 the same week
 * @note
 * @param  cal_time: Calendar time
 * @param  current_time: Real world time
 * @retval int (difference between real wordl time and abitrary time);
}
 */
int calc_in_week(time_t cal_time, time_t current_time) {
    time_t current_cal_time = get_cal_time_from_day_time(current_time);
    return cal_time - current_cal_time;
}

/**
 * @brief  Loads utc time converted from epoch time_t
 * @note
 * @param  *buf: String to be loaded
 * @param  *pre_str: String to be placed in front of utc time
 * @param  time: Epoch time to be converted
 * @retval None
 */
void load_epoch_to_utc(char *buf, char *pre_str, time_t time) {
    struct tm *tm = localtime(&time);
    sprintf(buf, "%s%02d%02d%02dT%02d%02d%02d",
            pre_str,
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec);
}

/************************************************************************* Static functions */

/**
 * @brief  Returns true year of given time
 * @note
 * @param  *tm: Struct tm to extract true year from
 * @retval int (true year of time)
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
 * @brief  Returns true week of given time
 * @note
 * @param  *tm: Struct tm to extract true week from
 * @retval int (true week of time)
 */
static int get_week(struct tm *tm) {
    mktime(tm);
    int DayOfTheWeek = (tm->tm_wday + (DAYS_IN_WEEK - 1)) % DAYS_IN_WEEK;
    tm->tm_mday -= DayOfTheWeek;
    tm->tm_mday += 3;
    mktime(tm);
    return tm->tm_yday / DAYS_IN_WEEK + 1;
}
