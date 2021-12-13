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
#include "../headers/time_handler.h"

/************************************************************************* Symbolic constants */

#define CAL_W 108
#define CAL_H 30
#define COLUMN_W 13

/************************************************************************* Static function prototypes */

static void load_into_arr(char* location, char* str);
static void prn_event_line(char* title, char* corner, int hour, int mins, int title_enabled);
static void prn_assignment(char* title, char* loc);

/************************************************************************* Global functions  */

/**
 * @brief Prints calender complete with inputted events and assignments
 * @note
 * @param  current_cal: Calendar to be shown
 * @retval None
 */
void prn_cal(calendar* current_cal) {
    /*     system("clear");
     */

    *current_cal = get_cal(current_cal->time);

    char pixel_arr[CAL_H][CAL_W] = {
        /*0         1         2         3         4         5         6         7         8         9         10     */
        /*01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456*/
        {"        ,-------------------------------------------------------------------------------------------------,\0"},
        {"        |                                                                                                 |\0"},
        {"        |-------------------------------------------------------------------------------------------------|\0"},
        {"        |             |             |             |             |             |             |             |\0"},
        {",-------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|\0"},
        {"| 00:00 |             |             |             |             |             |             |             |\0"},
        {"| 01:00 |             |             |             |             |             |             |             |\0"},
        {"| 02:00 |             |             |             |             |             |             |             |\0"},
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
    char* day_corners[DAYS_IN_WEEK] = {
        &pixel_arr[4][9],
        &pixel_arr[4][23],
        &pixel_arr[4][37],
        &pixel_arr[4][51],
        &pixel_arr[4][65],
        &pixel_arr[4][79],
        &pixel_arr[4][93],
    };

    int year = get_t_data(current_cal->time, t_year);
    int week = get_t_data(current_cal->time, t_week);

    char cal_header_buf[20];
    sprintf(cal_header_buf, "Week %-2d year %d", week, year);
    load_into_arr(cal_header_loc, cal_header_buf);

    int i;

    char date_buf[30];
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        char day_str[4];
        load_dow_string(day_str, i);

        sprintf(date_buf, "%s %2d/%-2d", day_str, get_t_data(current_cal->days[i].time, t_dom), get_t_data(current_cal->days[i].time, t_mon));
        load_into_arr(date_locs[i], date_buf);
    }

    int j;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        for (j = 0; j < CONTENT_IN_DAY; j++) {
            event event = current_cal->days[i].events[j];
            if (event.valid) {
                int start_hour = get_t_data(event.start_time, t_hour);
                int start_mins = get_t_data(event.start_time, t_min);
                int end_hour = get_t_data(event.end_time, t_hour);
                int end_mins = get_t_data(event.end_time, t_min);
                int title_enabled = (end_hour * MINS_IN_HOUR + end_mins) - (start_hour * MINS_IN_HOUR + start_mins) >= 120;

                prn_event_line(event.title, day_corners[i], start_hour, start_mins, title_enabled);
                prn_event_line(event.title, day_corners[i], end_hour, end_mins, 0);
            }
        }

        for (j = 0; j < CONTENT_IN_DAY; j++) {
            assignment assignment = current_cal->days[i].assignments[j];
            if (assignment.valid) {
                int hour = get_t_data(assignment.deadline, t_hour);
                char* loc = day_corners[i] + hour * CAL_W;
                prn_assignment(assignment.title, loc);
            }
        }
    }

    for (i = 0; i < CAL_H; i++) {
        printf("%s\n", pixel_arr[i]);
    }
}

/************************************************************************* Static functions */

/**
 * @brief Determines which event outline to use and loads it into calendar pixel array
 * @note Only inserts line at :30 or :00
 * @param  title: Title of event
 * @param  corner: Pointer to pixel array day column top left corner
 * @param  hour: Hour of outline
 * @param  mins: Minutes of outline
 * @param  title_enabled: True if event is longer then two hours and has room for title
 * @retval None
 */
static void prn_event_line(char* title, char* corner, int hour, int mins, int title_enabled) {
    int dashed = mins == 30;
    char* loc = corner + (hour + dashed) * CAL_W;
    char current = *(loc + 1);

    if (hour != 0) {
        if (current == '-' && !dashed) {
            load_into_arr(loc, "=============");
        } else if (dashed) {
            load_into_arr(loc, "-------------");
        } else {
            load_into_arr(loc, "_____________");
        }
    }

    if (title_enabled) {
        if (strlen(title) > COLUMN_W) {
            char title_buf[COLUMN_W + 1];
            strncpy(title_buf, title, COLUMN_W - 3);
            sprintf(title_buf + COLUMN_W - 3, "%s", "...");
            load_into_arr(loc + CAL_W, title_buf);
        } else {
            load_into_arr(loc + CAL_W, title);
        }
    }
}

/**
 * @brief Loads assignments into calendar pixel array
 * @note
 * @param title: Title of assignment
 * @param loc: Pointer to loading position in calendar pixel array
 * @retval None
 */
static void prn_assignment(char* title, char* loc) {
    char title_buf[20];
    if (strlen(title) > COLUMN_W - 2) {
        title_buf[0] = '{';
        strncpy(title_buf + 1, title, COLUMN_W - 5);
        sprintf(title_buf + COLUMN_W - 4, "...}");
        load_into_arr(loc + CAL_W, title_buf);
    } else {
        sprintf(title_buf, "{%s}", title);
        load_into_arr(loc + CAL_W, title_buf);
    }
}

/**
 * @brief Loads/overwrites string into calendar pixel array
 * @note
 * @param location: Pointer postion in calendar pixel array
 * @param str: String to be loaded
 * @retval None
 */

static void load_into_arr(char* location, char* str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        location[i] = str[i];
    }
}