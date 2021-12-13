#ifndef DATATYPES_H
#define DATATYPES_H

/************************************************************/

#include <time.h>

/************************************************************/

#define DAYS_IN_WEEK 7
#define HOURS_IN_DAY 24
#define HOURS_IN_WEEK HOURS_IN_DAY *DAYS_IN_WEEK
#define MINS_IN_HOUR 60
#define SECS_IN_MIN 60
#define SECS_IN_HOUR (MINS_IN_HOUR * SECS_IN_MIN)
#define SECS_IN_DAY HOURS_IN_DAY *MINS_IN_HOUR *SECS_IN_MIN

#define CONTENT_IN_DAY (HOURS_IN_DAY * 2)
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
    event events[CONTENT_IN_DAY];
    assignment assignments[CONTENT_IN_DAY];
} day;

typedef struct calendar {
    time_t time;
    int valid;
    day days[DAYS_IN_WEEK];
} calendar;

/************************************************************/

#endif