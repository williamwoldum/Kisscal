#ifndef DATATYPES_H
#define DATATYPES_H

/************************************************************/

#include <time.h>

/************************************************************/

#define DAYS_IN_WEEK 7

/************************************************************/

typedef struct event {
    char *title;
    time_t duration_start;
    time_t duration_end;
} event;

typedef struct day {
    int month;
    int day_of_month;
    event events[1];
} day;

typedef struct calendar {
    int year;
    int week;
    int valid;
    day days[DAYS_IN_WEEK];
} calendar;

/************************************************************/

#endif