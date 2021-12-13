#include "../headers/analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/time_handler.h"

/************************************************************************* Static function prototypes */

static void load_free_time_arr(int *secs_free, int *secs_non_events, calendar *cal, time_t current_time, int in_week);
static void prn_hour_use(time_t current_time, int in_week);
static void prn_loading_bar(float used, float total);
static void prn_free_hour_use(calendar *cal, time_t current_time, int in_week);
static void prn_assignments_status(calendar *cal, time_t current_time, int in_week);

/************************************************************************* Global functions  */

/**
 * @brief Analyzes calendar events and prints output
 * @note
 * @param cal: Calender to analyze
 * @retval None
 */
void analyze(calendar *cal) {
    time_t current_time = time(NULL);

    printf("\n-----------------------------------------------------------------------------------------------------------\n\n");
    char day_str[4];
    load_dow_string(day_str, get_t_data(current_time, t_dow));
    printf("It is currently %s %02d:%02d %d/%d week: %d year: %d\n",
           day_str,
           get_t_data(current_time, t_hour),
           get_t_data(current_time, t_min),
           get_t_data(current_time, t_dom),
           get_t_data(current_time, t_mon),
           get_t_data(current_time, t_week),
           get_t_data(current_time, t_year));

    int in_week = calc_in_week(cal->time, current_time);
    prn_hour_use(current_time, in_week);
    prn_free_hour_use(cal, current_time, in_week);
    prn_assignments_status(cal, current_time, in_week);

    printf("\n-----------------------------------------------------------------------------------------------------------\n");
}

/************************************************************************* Static functions */

/**
 * @brief  Loads total free time in week and free time left of week into given arrays
 * @note
 * @param  secs_free: Array representing free senconds every day. Accounts for current time in week
 * @param  secs_non_events: Array representing free senconds every day
 * @param  cal: Calender to analyze
 * @param  current_time: Current time
 * @param  in_week: If negative the cal is before the current week, positive after, 0 the same week
 * @retval None
 */
static void load_free_time_arr(int *secs_free, int *secs_non_events, calendar *cal, time_t current_time, int in_week) {
    int dow;
    if (in_week == 0) {
        dow = get_t_data(current_time, t_dow);
    } else if (in_week < 0) {
        dow = DAYS_IN_WEEK;
    } else {
        dow = 0;
    }

    int i, j;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        if (i < dow) {
            secs_free[i] = 0;
        } else if (in_week == 0 && i == dow) {
            int mins = get_t_data(current_time, t_min);
            mins += get_t_data(current_time, t_hour) * MINS_IN_HOUR;
            secs_free[i] = SECS_IN_DAY - mins * SECS_IN_MIN;
        } else {
            secs_free[i] = SECS_IN_DAY;
        }

        secs_non_events[i] = SECS_IN_DAY;

        for (j = 0; j < CONTENT_IN_DAY; j++) {
            event event = cal->days[i].events[j];
            if (event.valid && event.end_time > current_time) {
                if (current_time > event.start_time && current_time < event.end_time) {
                    secs_free[i] -= event.end_time - current_time;
                } else {
                    secs_free[i] -= event.end_time - event.start_time;
                }
            }
            secs_non_events[i] -= event.end_time - event.start_time;
        }
    }
}

/**
 * @brief  Calculates hours used and left of week and prints visuals
 * @note
 * @param  current_time: Current time
 * @param  in_week: If negative the cal is before the current week, positive after, 0 the same week
 * @retval None
 */
static void prn_hour_use(time_t current_time, int in_week) {
    float hours;
    if (in_week == 0) {
        hours = get_t_data(current_time, t_hour);
        int mins = get_t_data(current_time, t_min);
        hours += (float)mins / MINS_IN_HOUR;
        hours += get_t_data(current_time, t_dow) * HOURS_IN_DAY;
    } else if (in_week < 0) {
        hours = HOURS_IN_WEEK;
    } else {
        hours = 0;
    }

    printf("\nYou have spent %.1f hour(s) of %d total\n%.1f hour(s) remain\n",
           hours,
           HOURS_IN_WEEK,
           HOURS_IN_WEEK - hours);
    prn_loading_bar(hours, HOURS_IN_WEEK);
}

/**
 * @brief  Prints loading bar
 * @note
 * @param  used: units used
 * @param  total: total units
 * @retval None
 */
static void prn_loading_bar(float used, float total) {
    int num_tiles = 50;
    float ratio = (float)used / total;
    int used_tiles = (float)num_tiles * ratio;

    printf("[");
    int i;
    for (i = 0; i < num_tiles; i++) {
        if (i < used_tiles) {
            printf("#");
        } else {
            printf("-");
        }
    }

    printf("] | %.0f:%.0f\n", ratio * 100, 100 - ratio * 100);
}

/**
 * @brief  Calculates free hours used and left of week and prints visuals
 * @note
 * @param  cal: Calender to analyze
 * @param  current_time: Current time
 * @param  in_week: If negative the cal is before the current week, positive after, 0 the same week
 * @retval None
 */
static void prn_free_hour_use(calendar *cal, time_t current_time, int in_week) {
    int secs_free[DAYS_IN_WEEK], secs_non_events[DAYS_IN_WEEK];
    load_free_time_arr(secs_free, secs_non_events, cal, current_time, in_week);

    float left = 0;
    float total = 0;

    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        left += (float)secs_free[i] / SECS_IN_HOUR;
        total += (float)secs_non_events[i] / SECS_IN_HOUR;
    }
    printf("\nYou have spent %.1f free hour(s) of %.1f total\n%.1f free hour(s) remain\n",
           total - left,
           total,
           left);
    prn_loading_bar(total - left, total);

    if (in_week >= 0) {
        float sleep_time_left = (DAYS_IN_WEEK - (get_t_data(current_time, t_dow) + 1)) * 8 + 4;
        printf("Assuming you sleep 8 hours a night, you really have %.1f free hour(s) left\n", left - sleep_time_left);
    }
}

/**
 * @brief  Calculates average time to spend on assignment before deadline and print the info
 * @note
 * @param  cal: Calender to analyze
 * @param  current_time: Current time
 * @param  in_week: If negative the cal is before the current week, positive after, 0 the same week
 * @retval None
 */
static void prn_assignments_status(calendar *cal, time_t current_time, int in_week) {
    int i, j, at_least_one = 0;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        int days_to_deadline;
        int deadline_dow = get_t_data(cal->days[i].time, t_dow);
        if (in_week == 0) {
            days_to_deadline = deadline_dow - get_t_data(current_time, t_dow) + 1;
        } else if (in_week < 0) {
            days_to_deadline = -1;
        } else {
            days_to_deadline = deadline_dow;
        }

        void sort_content(day);
        for (j = 0; j < CONTENT_IN_DAY; j++) {
            assignment assignment = cal->days[i].assignments[j];
            if (assignment.valid && days_to_deadline >= 0) {
                at_least_one = 1;
                float avg_work_time = (assignment.expected_time - assignment.elapsed_time) / days_to_deadline;
                printf("\nTo finish your '%s' assignment, work on it for %.1f hour(s) every day until deadline",
                       assignment.title,
                       avg_work_time);
            }
        }
    }
    if (at_least_one) {
        printf("\n");
    }
}