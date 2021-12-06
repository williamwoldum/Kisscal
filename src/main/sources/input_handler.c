#include "../headers/input_handler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/regex_handler.h"

#define INPUT_BUFF_SIZE 100

void show_help() {
    printf(
        "\n"
        "********************************************************************************************\n"
        "open calendar <week> <year>\n"
        "clear calendar <week> <year>\n\n"

        "next week\n"
        "previous week\n\n"

        "clear day <day>\n\n"

        "add event '<event name>' <day> <time start> <time end> OR <time start> <duration>\n"
        "remove event '<event name>' <day>\n\n"

        "add assignment '<assingment name>' <day> <hand in> <duration>\n"
        "remove assignment '<assingment name>' <day>\n\n"

        "analyze\n\n"

        "close\n"
        "********************************************************************************************\n");
}

void prompt_user_intput(regex_t *regexs) {
    char user_input[INPUT_BUFF_SIZE];

    /* while loop to be deleted */
    int run = 1;
    while (run) {
        printf("\n>> ");
        fgets(user_input, INPUT_BUFF_SIZE, stdin);
        int match = checkregex(user_input, regexs);
        switch (match) {
            case invalid_input_rule:
                printf("Invalid input\n");
                break;
            case open_calendar_rule: {
                int week, year;

                sscanf(user_input + 13, " %d %d", &week, &year);

                calendar cal = get_cal(week, year);
                save_cal(cal);
                prn_file_content();
                break;
            }
            case clear_calendar_rule: {
                int week, year;

                sscanf(user_input + 14, " %d %d", &week, &year);
                delete_cal(week, year);
                break;
            }
            case next_week_rule: {
                break;
            }
            case previous_week_rule: {
                break;
            }
            case clear_day_rule: {
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};

                sscanf(user_input + 9, " %s", day_str);
                break;
            }
            case add_event_rule: {
                char event_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};
                char time_start_str[6] = {"\0\0\0\0\0\0"};
                char time_end_or_duration_str[6] = {"\0\0\0\0\0\0"};

                sscanf(user_input + 11, " %[^']' %s %s %s", event_str, day_str, time_start_str, time_end_or_duration_str);
                break;
            }
            case remove_event_rule: {
                char event_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};

                sscanf(user_input + 14, " %[^']' %s", event_str, day_str);
                break;
            }
            case add_assignment_rule: {
                char assignment_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};
                char hand_in_str[5] = {"\0\0\0\0\0"};
                char duration_str[5] = {"\0\0\0\0\0"};

                sscanf(user_input + 16, " %[^']' %s %s %s", assignment_str, day_str, hand_in_str, duration_str);
                break;
            }
            case remove_assignment_rule: {
                char assignment_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};

                sscanf(user_input + 19, " %[^']' %s", assignment_str, day_str);
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
                exit(EXIT_SUCCESS);
                break;
            }
            default: {
                printf("Error\n");
            }
        }
    }
}
