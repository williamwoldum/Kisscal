#include <regex.h>
#include <stdio.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/input_handler.h"
#include "../headers/regex_handler.h"

int main() {
    regex_t regexs[NUM_RULES];
    setup_regex(regexs);

    clr_file();
    calendar cal = get_cal(45, 2021);
    cal = get_cal(44, 2021);
    cal = get_cal(43, 2021);
    cal = get_cal(44, 2021);
    cal = get_cal(47, 2021);
    cal = get_cal(45, 2021);
    cal = get_cal(44, 2021);
    cal = get_cal(49, 2021);
    cal = get_cal(46, 2021);
    cal = get_cal(45, 2021);
    cal = get_cal(44, 2021);
    cal = get_cal(43, 2021);
    cal = get_cal(44, 2021);
    cal = get_cal(47, 2021);
    cal = get_cal(47, 2021);
    cal = get_cal(45, 2021);
    cal = get_cal(44, 2021);
    cal = get_cal(48, 2021);
    cal = get_cal(46, 2021);

    cal = get_cal(46, 2021);
    cal.days[0].dom = 1;
    save_cal(cal);

    printf("\n size of cal: %ld\n", sizeof(cal));

    delete_cal(49, 2021);

    prn_file_content();

    prompt_user_intput(regexs);

    return (0);
}