#ifndef DATATYPES_H
#define DATATYPES_H

/************************************************************/

#include <time.h>

/************************************************************/

#define DAYS_IN_WEEK 7
#define HOURS_IN_DAY 25
#define TITLE_LENGTH 20

/************************************************************/

typedef struct event {
    char title[TITLE_LENGTH];
    time_t duration_start;
    time_t duration_end;
    int valid;
} event;

typedef struct day {
    int month;
    int dom;
    event events[HOURS_IN_DAY];
} day;

typedef struct calendar {
    int year;
    int week;
    int valid;
    day days[DAYS_IN_WEEK];
} calendar;

/************************************************************/

#endif