#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/ics_handler.h"
#include "../headers/input_handler.h"
#include "../headers/regex_handler.h"

int main() {
    srand(time(NULL));

    regex_t regexs[num_input_rules];
    setup_regex(regexs);
    setup_renderer();

    clr_file();

    prn_cal();

    int run = 1;
    while (run) {
        run = prompt_user_input(regexs);
    }

    return (0);
}