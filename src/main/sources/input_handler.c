#include "../headers/input_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/regex_handler.h"
#include "../headers/time_handler.h"

#define INPUT_BUFF_SIZE 100

static void prn_help(void);
static int get_dow_from_str(char *str);

int prompt_user_input(void) {
    printf("\n>> ");

    char user_input[INPUT_BUFF_SIZE];
    fgets(user_input, INPUT_BUFF_SIZE, stdin);
    int match = checkregex(user_input);

    int run = 1;

    switch (match) {
        case invalid_input_rule:
            printf("Invalid input (type 'help' to see commands)\n");
            break;
        case open_week_rule: {
            int week, year;
            sscanf(user_input + 10, " %d %d", &week, &year);
            time_t cal_time = get_cal_time_from_week_and_year(week, year);
            current_cal = get_cal(cal_time);
            prn_cal();
            break;
        }
        case clear_week_rule: {
            delete_cal(current_cal.time);
            current_cal = get_cal(current_cal.time);
            break;
        }
        case next_week_rule: {
            int year = get_t_data(current_cal.time, t_year);
            int week = get_t_data(current_cal.time, t_week);
            time_t cal_time = get_cal_time_from_week_and_year(week + 1, year);
            current_cal = get_cal(cal_time);
            prn_cal();
            break;
        }
        case previous_week_rule: {
            int year = get_t_data(current_cal.time, t_year);
            int week = get_t_data(current_cal.time, t_week);
            time_t cal_time = get_cal_time_from_week_and_year(week - 1, year);
            current_cal = get_cal(cal_time);
            prn_cal();
            break;
        }
        case clear_day_rule: {
            char day_str[10];
            sscanf(user_input + 9, " %s", day_str);
            prn_cal();
            break;
        }
        case add_event_rule: {
            char event_str[TITLE_LENGTH];
            char day_str[10];
            char time_start_str[6];
            char time_end_str[6];
            sscanf(user_input + 11, " %[^']' %s %s %s", event_str, day_str, time_start_str, time_end_str);
            int dow = get_dow_from_str(day_str);

            int hour_start, mins_start;
            sscanf(time_start_str, "%d:%d", &hour_start, &mins_start);
            time_t time_start = digi_time_to_time_t(current_cal.days[dow].time, hour_start, mins_start);

            int hour_end, mins_end;
            sscanf(time_end_str, "%d:%d", &hour_end, &mins_end);
            time_t time_end = digi_time_to_time_t(current_cal.days[dow].time, hour_end, mins_end);
            printf("start %d:%d, end %d:%d, day %d, title %s\n", hour_start, mins_start, hour_end, mins_end, dow, event_str);
            printf("%ld %ld\n", time_start, time_end);

            add_event(event_str, time_start, time_end);
            prn_cal();
            break;
        }
        case remove_event_rule: {
            char event_str[TITLE_LENGTH];
            char day_str[10];

            sscanf(user_input + 14, " %[^']' %s", event_str, day_str);
            prn_cal();
            break;
        }
        case add_assignment_rule: {
            char assignment_str[TITLE_LENGTH];
            char day_str[10];
            char hand_in_str[5];
            char duration_str[5];

            sscanf(user_input + 16, " %[^']' %s %s %s", assignment_str, day_str, hand_in_str, duration_str);
            prn_cal();
            break;
        }
        case remove_assignment_rule: {
            char assignment_str[TITLE_LENGTH];
            char day_str[10];

            sscanf(user_input + 19, " %[^ ']' %s", assignment_str, day_str);
            prn_cal();
            break;
        }
        case print_day_rule: {
            char day_str[10];

            sscanf(user_input + 9, " %s", day_str);

            int dow = get_dow_from_str(day_str);
            prn_day_content(current_cal.days[dow].time);
            break;
        }
        case import_ics_rule: {
            break;
        }
        case export_ics_rule: {
            break;
        }
        case analyze_rule: {
            break;
        }
        case help_rule: {
            prn_help();
            break;
        }
        case close_rule: {
            run = 0;
            break;
        }
        default: {
            printf("Error\n");
        }
    }

    return run;
}

static void prn_help(void) {
    printf(
        "\n"
        "-----------------------------------------------------------------------------------------------------------\n"
        "\n"
        "open week <week> <year>\n"
        "clear week\n\n"

        "next week\n"
        "previous week\n\n"

        "clear day <day>\n\n"

        "add event '<event name>' <day> <time start> <time end> OR <time start> <duration>\n"
        "remove event '<event name>' <day>\n\n"

        "add assignment '<assingment name>' <day> <hand in> <duration>\n"
        "remove assignment '<assingment name>' <day>\n\n"

        "print day <day>\n\n"

        "import ICS\n"
        "export ICS\n\n"

        "analyze\n\n"

        "close\n"
        "\n"
        "-----------------------------------------------------------------------------------------------------------\n");
}

static int get_dow_from_str(char *str) {
    int dow = -1;
    if (tolower(str[0]) == 'm') {
        dow = 0;
    } else if (tolower(str[0]) == 't' && tolower(str[1]) == 'u') {
        dow = 1;
    } else if (tolower(str[0]) == 'w') {
        dow = 2;
    } else if (tolower(str[0]) == 't' && tolower(str[1]) == 'h') {
        dow = 3;
    } else if (tolower(str[0]) == 'f') {
        dow = 4;
    } else if (tolower(str[0]) == 's' && tolower(str[1]) == 'a') {
        dow = 5;
    } else if (tolower(str[0]) == 's' && tolower(str[1]) == 'u') {
        dow = 6;
    }
    return dow;
}
