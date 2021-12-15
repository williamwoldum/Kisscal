#include "../headers/input_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/analyzer.h"
#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/ics_handler.h"
#include "../headers/regex_handler.h"
#include "../headers/time_handler.h"

/************************************************************************* Symbolic constants */

#define INPUT_BUFF_SIZE 100
#define TIME_LEN 6
#define DAY_LEN 10

/************************************************************************* Static function prototypes */

static int get_dow_from_str(char *str);
static void prn_day_content(time_t day_time);
static void prn_help(void);
static int cmp_events(const void *a_, const void *b_);
static int cmp_assignments(const void *a_, const void *b_);

/************************************************************************* Global functions  */
/**
 * @brief Promts the user for input, evaluates it and executes proper action
 * @note
 * @param  *current_cal: Calendar currently shown to user
 * @retval int
 */
int prompt_user_input(calendar *current_cal) {
    printf("\n>> ");

    char user_input[INPUT_BUFF_SIZE];
    fgets(user_input, INPUT_BUFF_SIZE, stdin);
    int match = checkregex(user_input);

    int run = 1;

    switch (match) {
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
            char title[TITLE_LENGTH];
            char day_str[DAY_LEN];
            char time_start_str[TIME_LEN];
            char time_end_str[TIME_LEN];

            sscanf(user_input, "%*[^']'%*[^']'  %s %s %s", day_str, time_start_str, time_end_str);
            load_capped_title(user_input, title, '\'', '\'', 0);
            int dow = get_dow_from_str(day_str);

            int hour_start, mins_start;
            sscanf(time_start_str, "%d:%d", &hour_start, &mins_start);
            time_t time_start = digi_time_to_epoch(current_cal->days[dow].time, hour_start, mins_start);

            int hour_end, mins_end;
            sscanf(time_end_str, "%d:%d", &hour_end, &mins_end);
            time_t time_end = digi_time_to_epoch(current_cal->days[dow].time, hour_end, mins_end);

            int succes = add_event(title, time_start, time_end);
            if (succes) {
                prn_cal(current_cal);
            }
            break;
        }
        case remove_event_rule: {
            char day_str[DAY_LEN];
            char time_str[TIME_LEN];

            sscanf(user_input, "%*[^:]: %s %s", day_str, time_str);

            int dow = get_dow_from_str(day_str);
            int hour, mins;
            sscanf(time_str, "%d:%d", &hour, &mins);
            time_t time_start = digi_time_to_epoch(current_cal->days[dow].time, hour, mins);

            delete_event(time_start);

            prn_cal(current_cal);
            break;
        }
        case add_assignment_rule: {
            char title[TITLE_LENGTH];
            char day_str[DAY_LEN];
            char deadline_str[TIME_LEN];
            float expexted_time;
            float elapsed_time;

            sscanf(user_input, "%*[^']'%*[^']' %s %s %f %f", day_str, deadline_str, &expexted_time, &elapsed_time);
            load_capped_title(user_input, title, '\'', '\'', 0);

            int dow = get_dow_from_str(day_str);
            int hour, mins;
            sscanf(deadline_str, "%d:%d", &hour, &mins);
            time_t time_deadline = digi_time_to_epoch(current_cal->days[dow].time, hour, mins);

            add_assignemnt(title, time_deadline, expexted_time, elapsed_time);

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
            time_t time_start = digi_time_to_epoch(current_cal->days[dow].time, hour, mins);

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
        case sync_rule: {
            import_ics();
            prn_cal(current_cal);
            break;
        }
        case export_rule: {
            convert_cal_to_ics(current_cal);
            break;
        }
        case analyze_rule: {
            analyze(current_cal);
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
        case invalid_input_rule:
        default:
            printf("Invalid input (type 'help' to see commands)\n");
    }

    return run;
}

/**
 * @brief Sorts content by time (events and assignments) in specific day
 * @note
 * @param  *day: Day of content to be sorted
 * @retval None
 */
void sort_content(day *day) {
    qsort(day->events, CONTENT_IN_DAY, sizeof(event), cmp_events);
    qsort(day->assignments, CONTENT_IN_DAY, sizeof(assignment), cmp_assignments);
}

/**
 * @brief  Loads title from string input and caps it to max length
 * @note
 * @param  *str: String with part to be capped
 * @param  *dest: String to be loaded with capped title
 * @param  start_delim: Delimter of where the title starts
 * @param  end_delim: Delimter of where the title ends
 * @retval None
 */
void load_capped_title(char *str, char *dest, char start_delim, char end_delim, int ics_case) {
    char *title_start = strchr(str, start_delim) + 1;
    char *title_end = strchr(title_start, end_delim);

    int title_length = title_end - title_start;
    if (title_length > TITLE_LENGTH - 1) {
        title_length = TITLE_LENGTH - 1;
    }

    memcpy(dest, title_start, title_length);

    if (ics_case) {
        dest[title_length - 1] = '\0';
    } else {
        dest[title_length] = '\0';
    }
}

/************************************************************************* Static functions */

/**
 * @brief  Returns day index of the week from day string
 * @note  Case insensitive
 * @param  *str: String to be considered
 * @retval int 0-6
 */
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

/**
 * @brief  Prints content (events and assignments) in a time sortet list from specific/chosen day
 * @note
 * @param  day_time: 12:00 am on day, acts as id
 * @retval None
 */
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
    for (i = 0; i < CONTENT_IN_DAY; i++) {
        event event = day.events[i];
        if (event.valid) {
            printf("%02d:%02d to %02d:%02d | '%s'\n",
                   get_t_data(event.start_time, t_hour),
                   get_t_data(event.start_time, t_min),
                   get_t_data(event.end_time, t_hour),
                   get_t_data(event.end_time, t_min),
                   event.title);
        }
    }

    printf("\nAssignments:\n");
    for (i = 0; i < CONTENT_IN_DAY; i++) {
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

/**
 * @brief Prints help guide for user
 * @note
 * @retval None
 */
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

        "add assignment: '<assignment name>' <day> <deadline> <expected time> <used time>\n"
        "remove assignment: <day> <deadline>\n\n"

        "print: <day>\n\n"

        "sync\n"
        "export\n\n"

        "analyze\n\n"

        "close\n"
        "\n"
        "-----------------------------------------------------------------------------------------------------------\n");
}

/**
 * @brief Compare events by start time
 * @note
 * @param  *a_: Points to element(event) a to comparement
 * @param  *b_: Points to element(event) b to comparement
 * @retval int (return differens in time)
 */
static int cmp_events(const void *a_, const void *b_) {
    event a = *(event *)a_;
    event b = *(event *)b_;

    return a.start_time - b.start_time;
}
/**
 * @brief Compare assignments by deadline time
 * @note
 * @param  *a_: Points to element(assignment) a to comparement
 * @param  *b_: Points to element(assignment) b to comparement
 * @retval int (return differens in time)
 */
static int cmp_assignments(const void *a_, const void *b_) {
    assignment a = *(assignment *)a_;
    assignment b = *(assignment *)b_;

    return a.deadline - b.deadline;
}