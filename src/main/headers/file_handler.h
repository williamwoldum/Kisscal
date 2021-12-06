#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/************************************************************/

#include "datatypes.h"

/************************************************************/

calendar get_cal(int week, int year);
void delete_cal(int week, int year);
void clear_day(int dow, int week, int year);
void add_event(int dow, int week, int year, char *title, time_t start_time, time_t end_time);

void clr_file();
void prn_file_content();
void prn_cal_dates(calendar *cal);

/************************************************************/

#endif