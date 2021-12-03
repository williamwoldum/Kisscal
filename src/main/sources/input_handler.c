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

void pull_out_values(char user_input[], char *str, int index) {
    int start_i = 0, end_i = 1;
    int count = 0;
    int i = 0, j = 0;
    for (i = 0; user_input[i] != '\0'; i++) {
        if (user_input[i] == ' ' || user_input[i] == '\n') {
            count++;
            if (count == index) {
                if (user_input[i + 1] == '\'') {
                    start_i = i + 2;
                } else {
                    start_i = i + 1;
                }
            } else if (count == index + 1) {
                if (user_input[i - 1] == '\'') {
                    end_i = i - 2;
                } else {
                    end_i = i - 1;
                }
            }
        }
    }
    for (i = start_i, j = 0; i <= end_i; i++, j++) {
        str[j] = user_input[i];
    }
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
                char week_str[3] = {"\0\0\0"};
                char year_str[5] = {"\0\0\0\0\0"};

                pull_out_values(user_input, week_str, 2);
                pull_out_values(user_input, year_str, 3);

                /* int week = atoi(week_str);
                int year = atoi(year_str);

                calendar cal = get_cal(week, year); */

                prn_file_content();
                break;
            }
            case clear_calendar_rule: {
                char week_str[3] = {"\0\0\0"};
                char year_str[5] = {"\0\0\0\0\0"};

                pull_out_values(user_input, week_str, 2);
                pull_out_values(user_input, year_str, 3);

                int week = atoi(week_str);
                int year = atoi(year_str);

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

                pull_out_values(user_input, day_str, 2);
                break;
            }
            case add_event_rule: {
                char event_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};
                char time_start_str[6] = {"\0\0\0\0\0\0"};
                char time_end_or_duration_str[6] = {"\0\0\0\0\0\0"};

                pull_out_values(user_input, event_str, 2);
                pull_out_values(user_input, day_str, 3);
                pull_out_values(user_input, time_start_str, 4);
                pull_out_values(user_input, time_end_or_duration_str, 5);
                break;
            }
            case remove_event_rule: {
                char event_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};
                pull_out_values(user_input, event_str, 2);
                pull_out_values(user_input, day_str, 3);
                break;
            }
            case add_assignment_rule: {
                char assignment_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};
                char hand_in_str[5] = {"\0\0\0\0\0"};
                char duration_str[5] = {"\0\0\0\0\0"};

                pull_out_values(user_input, assignment_str, 2);
                pull_out_values(user_input, day_str, 3);
                pull_out_values(user_input, hand_in_str, 4);
                pull_out_values(user_input, duration_str, 5);
                break;
            }
            case remove_assignment_rule: {
                char assignment_str[TITLE_LENGTH] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};
                char day_str[9] = {"\0\0\0\0\0\0\0\0\0"};

                pull_out_values(user_input, assignment_str, 2);
                pull_out_values(user_input, day_str, 3);
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
