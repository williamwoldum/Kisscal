#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/************************************************************/

#include <stdio.h>

#include "../header_files/datatypes.h"

/************************************************************/

calendar get_cal(int week, int year);
void save_cal(calendar cal);
void delete_cal(int week, int year);

static int get_cal_index(int week, int year, FILE *file);
static void handle_empty_file(FILE *file);
static calendar create_cal(int week, int year, FILE *file);
static void fill_cal_with_dates(calendar *);
static void save_new_cal(calendar cal, FILE *file);
static int inc_cal_counter(FILE *file);

void clr_file();
void prn_file_content();
void prn_cal_dates(calendar cal);

/************************************************************/

#endif