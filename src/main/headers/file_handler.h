#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/************************************************************/

#include "datatypes.h"

/************************************************************/

calendar get_cal(int week, int year);
void save_cal(calendar cal);
void delete_cal(int week, int year);

void clr_file();
void prn_file_content();
void prn_cal_dates(calendar cal);

/************************************************************/

#endif