#ifndef COMPARE_HELPER_H
#define COMPARE_HELPER_H

/************************************************************/

#include "../../../main/headers/datatypes.h"

/************************************************************/

int cmp_cals(calendar cal1, calendar cal2);
int cmp_days(day day1, day day2);
int cmp_events(event event1, event event2);
int cmp_assignments(assignment assignment1, assignment assignment2);

/************************************************************/

#endif