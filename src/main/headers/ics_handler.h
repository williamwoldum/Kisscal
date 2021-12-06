#ifndef ICS_HANDLER_H
#define ICS_HANDLER_H

/************************************************************/

#include "datatypes.h"

/************************************************************/

void convert_cal_to_ics(calendar cal);
char *get_DTSTAMP(calendar, int, int);
char *get_dtstart_dtend(event, int);
int get_uid();

/************************************************************/

#endif