#include "../headers/input_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/ics_handler.h"
#include "../headers/regex_handler.h"
#include "../headers/time_handler.h"

#define INPUT_BUFF_SIZE 100
#define TIME_LEN 6
#define DAY_LEN 10

static void prn_help(void);
static int get_dow_from_str(char* str);
static void prn_day_content(time_t day_time);
static void sort_content(day* day);
static int cmp_events(const void* a_, const void* b_);
static int cmp_assignments(const void* a_, const void* b_);

int prompt_user_input(calendar* current_cal) {
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
            sscanf(user_input, "%*[^:]: %d %d", &week, &year);
            time_t cal_time = get_cal_time_from_week_and_year(week, year);
            printf("\ncurrent cal from input %ld\n", cal_time);
            printf("time 1 %ld\n", current_cal->time);
            *current_cal = get_cal(cal_time);
            printf("time 2 %ld\n", current_cal->time);
            prn_cal(current_cal);
            break;
        }
        case clear_week_rule: {
            delete_cal(current_cal->time);
            *current_cal = get_cal(current_cal->time);
            prn_cal(current_cal);
            break;
        }
        case next_week_rule: {
            int year = get_t_data(current_cal->time, t_year);
            int week = get_t_data(current_cal->time, t_week);
            time_t cal_time = get_cal_time_from_week_and_year(week + 1, year);
            *current_cal = get_cal(cal_time);
            prn_cal(current_cal);
            break;
        }
        case previous_week_rule: {
            int year = get_t_data(current_cal->time, t_year);
            int week = get_t_data(current_cal->time, t_week);
            time_t cal_time = get_cal_time_from_week_and_year(week - 1, year);
            *current_cal = get_cal(cal_time);
            prn_cal(current_cal);
            break;
        }
        case clear_day_rule: {
            char day_str[DAY_LEN];
            sscanf(user_input, "%*[^:]: %s", day_str);
            int dow = get_dow_from_str(day_str);
            clear_day(current_cal->days[dow].time);
            prn_cal(current_cal);
            break;
        }
        case add_event_rule: {
            char event_str[TITLE_LENGTH];
            char day_str[DAY_LEN];
            char time_start_str[TIME_LEN];
            char time_end_str[TIME_LEN];
            sscanf(user_input, "%*[^:]: '%[^']'  %s %s %s", event_str, day_str, time_start_str, time_end_str);
            int dow = get_dow_from_str(day_str);

            int hour_start, mins_start;
            sscanf(time_start_str, "%d:%d", &hour_start, &mins_start);
            time_t time_start = digi_time_to_time_t(current_cal->days[dow].time, hour_start, mins_start);

            int hour_end, mins_end;
            sscanf(time_end_str, "%d:%d", &hour_end, &mins_end);
            time_t time_end = digi_time_to_time_t(current_cal->days[dow].time, hour_end, mins_end);

            add_event(event_str, time_start, time_end);
            prn_cal(current_cal);
            break;
        }
        case remove_event_rule: {
            char day_str[DAY_LEN];
            char time_str[TIME_LEN];

            sscanf(user_input, "%*[^:]: %s %s", day_str, time_str);

            int dow = get_dow_from_str(day_str);
            int hour, mins;
            sscanf(time_str, "%d:%d", &hour, &mins);
            time_t time_start = digi_time_to_time_t(current_cal->days[dow].time, hour, mins);

            delete_event(time_start);

            prn_cal(current_cal);
            break;
        }
        case add_assignment_rule: {
            char assignment_str[TITLE_LENGTH];
            char day_str[DAY_LEN];
            char deadline_str[TIME_LEN];
            float expexted_time;
            float elapsed_time;

            sscanf(user_input, "%*[^:]: '%[^']' %s %s %f %f", assignment_str, day_str, deadline_str, &expexted_time, &elapsed_time);

            int dow = get_dow_from_str(day_str);
            int hour, mins;
            sscanf(deadline_str, "%d:%d", &hour, &mins);
            time_t time_deadline = digi_time_to_time_t(current_cal->days[dow].time, hour, mins);

            add_assignemnt(assignment_str, time_deadline, expexted_time, elapsed_time);

            prn_cal(current_cal);
            break;
        }
        case remove_assignment_rule: {
            char day_str[DAY_LEN];
            char time_str[TIME_LEN];

            sscanf(user_input, "%*[^:]: %s %s", day_str, time_str);

            int dow = get_dow_from_str(day_str);
            int hour, mins;
            sscanf(time_str, "%d:%d", &hour, &mins);
            time_t time_start = digi_time_to_time_t(current_cal->days[dow].time, hour, mins);

            delete_assignment(time_start);

            prn_cal(current_cal);
            break;
        }
        case print_day_rule: {
            char day_str[DAY_LEN];

            sscanf(user_input, "%*[^:]: %s", day_str);

            int dow = get_dow_from_str(day_str);
            prn_day_content(current_cal->days[dow].time);
            break;
        }
        case import_ics_rule: {
            break;
        }
        case export_ics_rule: {
            convert_cal_to_ics(current_cal);
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
        "open week: <week> <year>\n"
        "clear week\n\n"

        "next week\n"
        "previous week\n\n"

        "clear: <day>\n\n"

        "add event: '<event name>' <day> <time start> <time end>\n"
        "remove event: <day> <time start>\n\n"

        "add assignment: '<assingment name>' <day> <deadline> <expected time> <used time>\n"
        "remove assignment: <day> <deadline>\n\n"

        "print: <day>\n\n"

        "import ICS: <path>\n"
        "export ICS\n\n"

        "analyze\n\n"

        "close\n"
        "\n"
        "-----------------------------------------------------------------------------------------------------------\n");
}

static int get_dow_from_str(char* str) {
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

static void prn_day_content(time_t day_time) {
    time_t cal_time = get_cal_time_from_day_time(day_time);

    calendar cal = get_cal(cal_time);

    day day;
    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        if (cal.days[i].time == day_time) {
            day = cal.days[i];
        }
    }
    sort_content(&day);

    printf("\n-----------------------------------------------------------------------------------------------------------\n");
    printf("\nEvents:\n");
    for (i = 0; i < HOURS_IN_DAY * 2; i++) {
        event event = day.events[i];
        if (event.valid) {
            printf("%02d:%02d to %02d:%02d | '%s' %ld\n",
                   get_t_data(event.start_time, t_hour),
                   get_t_data(event.start_time, t_min),
                   get_t_data(event.end_time, t_hour),
                   get_t_data(event.end_time, t_min),
                   event.title,
                   event.start_time);
        }
    }

    printf("\nAssignments:\n");
    for (i = 0; i < HOURS_IN_DAY * 2; i++) {
        assignment assignment = day.assignments[i];
        if (assignment.valid) {
            printf("Deadline: %02d:%02d, expected time: %.1lf hour(s), used time: %.1lf hour(s) | '%s'\n",
                   get_t_data(assignment.deadline, t_hour),
                   get_t_data(assignment.deadline, t_min),
                   assignment.expected_time,
                   assignment.elapsed_time,
                   assignment.title);
        }
    }
    printf("\n-----------------------------------------------------------------------------------------------------------\n");
}

static void sort_content(day* day) {
    qsort(day->events, HOURS_IN_DAY * 2, sizeof(event), cmp_events);
    qsort(day->assignments, HOURS_IN_DAY * 2, sizeof(assignment), cmp_assignments);
}

static int cmp_events(const void* a_, const void* b_) {
    event a = *(event*)a_;
    event b = *(event*)b_;

    return a.start_time - b.start_time;
}

static int cmp_assignments(const void* a_, const void* b_) {
    assignment a = *(assignment*)a_;
    assignment b = *(assignment*)b_;

    return a.deadline - b.deadline;
}