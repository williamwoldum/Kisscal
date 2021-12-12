#include "../headers/analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/time_handler.h"

#define HOURS_IN_WEEEK (HOURS_IN_DAY * DAYS_IN_WEEK)
#define SECS_IN_DAY (HOURS_IN_DAY * 60 * 60)

void analyze(calendar *cal) {
    int cal_week = get_t_data(cal->time, t_week);
    int cal_year = get_t_data(cal->time, t_year);

    time_t current_time = time(NULL);
    int current_week = get_t_data(current_time, t_week);
    int current_year = get_t_data(current_time, t_year);

    int in_week = cal_week == current_week && cal_year == current_year;
    int dow = in_week ? get_t_data(current_time, t_dow) : 0;

    int mins_free[DAYS_IN_WEEK] = {SECS_IN_DAY};
    int i, j;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        if (i < dow) {
            mins_free[i] = 0;
        } else if (i == dow) {
            mins_free[i] -= current_time % SECS_IN_DAY;
        }

        for (j = 0; j < HOURS_IN_DAY * 2; j++) {
            event event = cal->days[i].events[j];
            if (event.valid) {
                if (in_week) {
                }
            }
        }
    }
}