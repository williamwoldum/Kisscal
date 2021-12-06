#include "../headers/cal_renderer.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/input_handler.h"
#include "../headers/regex_handler.h"

calendar current_cal;

char pixel_arr[29][108] = {
    /*0         1         2         3         4         5         6         7         8         9         10     */
    /*01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456*/
    {"        ,-------------------------------------------------------------------------------------------------,\0"},
    {"        |                                                                                                 |\0"},
    {"        |-------------------------------------------------------------------------------------------------|\0"},
    {"        |             |             |             |             |             |             |             |\0"},
    {",-------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|\0"},
    {"| 00:00 |             |             |             |             |             |             |             |\0"},
    {"| 01:00 |             |             |             |             |             |             |             |\0"},
    {"| 03:00 |             |             |             |             |             |             |             |\0"},
    {"| 04:00 |             |             |             |             |             |             |             |\0"},
    {"| 05:00 |             |             |             |             |             |             |             |\0"},
    {"| 06:00 |             |             |             |             |             |             |             |\0"},
    {"| 07:00 |             |             |             |             |             |             |             |\0"},
    {"| 08:00 |             |             |             |             |             |             |             |\0"},
    {"| 09:00 |             |             |             |             |             |             |             |\0"},
    {"| 10:00 |             |             |             |             |             |             |             |\0"},
    {"| 11:00 |             |             |             |             |             |             |             |\0"},
    {"| 12:00 |             |             |             |             |             |             |             |\0"},
    {"| 13:00 |             |             |             |             |             |             |             |\0"},
    {"| 14:00 |             |             |             |             |             |             |             |\0"},
    {"| 15:00 |             |             |             |             |             |             |             |\0"},
    {"| 16:00 |             |             |             |             |             |             |             |\0"},
    {"| 17:00 |             |             |             |             |             |             |             |\0"},
    {"| 18:00 |             |             |             |             |             |             |             |\0"},
    {"| 19:00 |             |             |             |             |             |             |             |\0"},
    {"| 20:00 |             |             |             |             |             |             |             |\0"},
    {"| 21:00 |             |             |             |             |             |             |             |\0"},
    {"| 22:00 |             |             |             |             |             |             |             |\0"},
    {"| 23:00 |             |             |             |             |             |             |             |\0"},
    {"'---------------------------------------------------------------------------------------------------------'\0"}};

char* cal_header_loc = &pixel_arr[1][49];
char* date_locs[DAYS_IN_WEEK] = {
    &pixel_arr[3][11],
    &pixel_arr[3][25],
    &pixel_arr[3][39],
    &pixel_arr[3][53],
    &pixel_arr[3][67],
    &pixel_arr[3][81],
    &pixel_arr[3][95],
};

char day_strs[DAYS_IN_WEEK][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void setup_renderer(void) {
    current_cal = get_cal(49, 2021);
}

/*  &pixel_arr[6][9],
  &pixel_arr[6][23],
  &pixel_arr[6][37],
  &pixel_arr[6][51],
  &pixel_arr[6][65],
  &pixel_arr[6][79],
  &pixel_arr[6][93],*/

void load_into_arr(char* location, char* str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        location[i] = str[i];
    }
}

void prn_cal(void) {
    system("clear");
    char cal_header_buf[20];
    sprintf(cal_header_buf, "Week %-2d year %d", current_cal.week, current_cal.year);
    load_into_arr(cal_header_loc, cal_header_buf);

    int i;

    char date_buf[30];
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        sprintf(date_buf, "%s %d/%2d", day_strs[i], current_cal.days[i].dom, current_cal.days[i].month);
        load_into_arr(date_locs[i], date_buf);
    }

    for (i = 0; i < 29; i++) {
        printf("%s\n", pixel_arr[i]);
    }
}
