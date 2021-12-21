#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/************************************************************/

#include "datatypes.h"

/************************************************************/

void prepare_file(int state);
calendar get_cal(time_t cal_time);
void delete_cal(time_t cal_time);
void clear_day(time_t day_time);
int add_event(char* title, time_t start_time, time_t end_time);
void delete_event(time_t start_time);
void add_assignment(char* title, time_t deadline, float expected_time, float elapsed_time);
void delete_assignment(time_t deadline);

void prn_file_content(void);

/************************************************************/

#endif
