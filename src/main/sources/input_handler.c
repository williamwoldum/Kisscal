#include "../headers/input_handler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/regex_handler.h"

#define INPUT_BUFF_SIZE 100

void show_help() {
    printf(
        "\n"
        "open calendar <week> <year>\n"
        "clear calendar <week> <year>\n\n"

        "next week\n"
        "previous week\n\n"

        "clear day <day>\n\n"

        "add event '<event name>' <day> <time start> <time end> OR <time start> <duration>\n"
        "add event '<event name>' <day> <time start> <time end> OR <time start> <duration>\n\n"

        "add assignment '<assingment name>' <day> <hand in> <duration>\n"
        "remove assignment '<assingment name>' <day>\n\n"

        "analyze\n\n"

        "close\n");
}

/*
void pull_out_values(char *str, int rule, int index) {
    int start_i = 0, end_i = 1;
    int i = 0;
}
*/

void prompt_user_intput(regex_t *regexs) {
    char user_input[INPUT_BUFF_SIZE];
    printf(">> ");
    fgets(user_input, INPUT_BUFF_SIZE, stdin);
    int match = checkregex(user_input, regexs);
    switch (match) {
        case invalid_input:
            printf("Invalid input\n");
            break;
        case open_calendar:
            // char *week;
            // char *year;
            // pull_out_values(week, open_calendar, 2);
            // pull_out_values(week, open_calendar, 3);
            //pull out values...
            //render calendar...
            break;
        case clear_calendar:
            //pull out values
            //reset calendar
            //render calendar
            break;
        case next_week:
            //render calendar
            break;
        case previous_week:
            //render calendar
            break;
        case clear_day:
            //reset day
            //render calendar
            break;
        case add_event:
            //pull out values
            //add event
            //render calendar
            break;
        case remove_event:
            //pull out values
            //reset event
            //render calendar
            break;
        case add_assignment:
            //pull out values
            //add assignment
            //render calendar
            break;
        case remove_assignment:
            //pull out values
            //remove assignment
            //render calendar
            break;
        case analyze:
            //analyze
            break;
        case help:
            show_help();
            break;
        case close:
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Error\n");
    }
}
