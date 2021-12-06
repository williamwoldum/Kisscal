#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/ics_handler.h"
#include "../headers/input_handler.h"
#include "../headers/regex_handler.h"

int main() {
    srand(time(NULL));

    regex_t regexs[num_input_rules];
    setup_regex(regexs);

    clr_file();
    calendar cal = get_cal(47, 2021);

    cal = get_cal(49, 2021);

    cal = get_cal(50, 2021);

    cal = get_cal(50, 2021);

    convert_cal_to_ics(get_cal(45, 2021));

    cal = get_cal(51, 2021);

    cal = get_cal(51, -2021);

    printf("\nSize of cal: %ld\n", sizeof(cal));
    prn_file_content();

    int run = 1;
    while (run) {
        run = prompt_user_input(regexs);
    }

    return (0);
}