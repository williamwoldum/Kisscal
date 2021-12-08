#ifndef DATATYPES_H
#define DATATYPES_H

/************************************************************/

#include <time.h>

/************************************************************/

#define DAYS_IN_WEEK 7
#define HOURS_IN_DAY 24
#define TITLE_LENGTH 16

/************************************************************/

typedef struct assignment {
    char title[TITLE_LENGTH];
    time_t deadline;
    float expected_time;
    float elapsed_time;
    int valid;
} assignment;

typedef struct event {
    char title[TITLE_LENGTH];
    time_t start_time;
    time_t end_time;
    int valid;
} event;

typedef struct day {
    time_t time;
    event events[HOURS_IN_DAY * 2];
    assignment assignments[HOURS_IN_DAY * 2];
} day;

typedef struct calendar {
    time_t time;
    int valid;
    day days[DAYS_IN_WEEK];
} calendar;

/************************************************************/

#endif