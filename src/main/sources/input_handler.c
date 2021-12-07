#include "../headers/input_handler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/regex_handler.h"

#define INPUT_BUFF_SIZE 100

void show_help() {
    printf(
        "\n"
        "-----------------------------------------------------------------------------------------------------------\n"
        "\n"
        "open calendar <week> <year>\n"
        "clear calendar\n\n"

        "next week\n"
        "previous week\n\n"

        "clear day <day>\n\n"

        "add event '<event name>' <day> <time start> <time end> OR <time start> <duration>\n"
        "remove event '<event name>' <day>\n\n"

        "add assignment '<assingment name>' <day> <hand in> <duration>\n"
        "remove assignment '<assingment name>' <day>\n\n"

        "analyze\n\n"

        "close\n"
        "\n"
        "-----------------------------------------------------------------------------------------------------------\n");
}

int prompt_user_input() {
    printf("\n>> ");

    char user_input[INPUT_BUFF_SIZE];
    fgets(user_input, INPUT_BUFF_SIZE, stdin);
    int match = checkregex(user_input);

    int run = 1;

    switch (match) {
        case invalid_input_rule:
            printf("Invalid input (type 'help' to see commands)\n");
            break;
        case open_calendar_rule: {
            int week, year;

            sscanf(user_input + 13, " %d %d", &week, &year);

            current_cal = get_cal(week, year);

            prn_cal();
            break;
        }
        case clear_calendar_rule: {
            delete_cal(current_cal.week, current_cal.year);
            current_cal = get_cal(week, year);
            break;
        }
        case next_week_rule: {
            current_cal = get_cal(current_cal.week + 1, current_cal.year);
            prn_cal();
            break;
        }
        case previous_week_rule: {
            current_cal = get_cal(current_cal.week - 1, current_cal.year);
            prn_cal();
            break;
        }
        case clear_day_rule: {
            char day_str[9];

            sscanf(user_input + 9, " %s", day_str);
            prn_cal();
            break;
        }
        case add_event_rule: {
            char event_str[TITLE_LENGTH];
            char day_str[9];
            char time_start_str[6];
            char time_end_or_duration_str[6];

            sscanf(user_input + 11, " %[^']' %s %s %s", event_str, day_str, time_start_str, time_end_or_duration_str);
            prn_cal();
            break;
        }
        case remove_event_rule: {
            char event_str[TITLE_LENGTH];
            char day_str[9];

            sscanf(user_input + 14, " %[^']' %s", event_str, day_str);
            prn_cal();
            break;
        }
        case add_assignment_rule: {
            char assignment_str[TITLE_LENGTH];
            char day_str[9];
            char hand_in_str[5];
            char duration_str[5];

            sscanf(user_input + 16, " %[^']' %s %s %s", assignment_str, day_str, hand_in_str, duration_str);
            prn_cal();
            break;
        }
        case remove_assignment_rule: {
            char assignment_str[TITLE_LENGTH];
            char day_str[9];

            sscanf(user_input + 19, " %[^ ']' %s", assignment_str, day_str);
            prn_cal();
            break;
        }
        case print_day_rule: {
            char day_str[9];

            sscanf(user_input + 9, " %s", day_str);
            printf("%s", day_str);
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
            show_help();
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
