#include "../headers/analyzer.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/time_handler.h"

#define HOURS_IN_WEEEK HOURS_IN_DAY *DAYS_IN_WEEK
#define SECS_IN_DAY HOURS_IN_DAY *MINS_IN_HOUR *SECS_IN_MINS

static void load_free_time_arr(int *mins_free, calendar *cal);

void analyze(calendar *cal) {
    int mins_free[DAYS_IN_WEEK];
    load_free_time_arr(mins_free, cal);

    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        printf("\n%d", mins_free[i] / SECS_IN_MINS);
    }
}

static void load_free_time_arr(int *mins_free, calendar *cal) {
    time_t current_time = time(NULL);
    time_t current_cal_time = get_cal_time_from_day_time(current_time);
    int in_week = cal->time - current_cal_time;

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
            mins_free[i] = 0;
        } else if (in_week == 0 && i == dow) {
            mins_free[i] = SECS_IN_DAY - fmod(current_time, SECS_IN_DAY);
        } else {
            mins_free[i] = SECS_IN_DAY;
        }

        for (j = 0; j < CONTENT_IN_DAY; j++) {
            event event = cal->days[i].events[j];
            if (event.valid && event.end_time > current_time) {
                if (current_time > event.start_time && current_time < event.end_time) {
                    mins_free[i] -= event.end_time - current_time;
                } else {
                    mins_free[i] -= (event.end_time - event.start_time);
                }
            }
        }
    }
}
