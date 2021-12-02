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
    calendar cal = get_cal(47, 2021);
    save_cal(cal);

    cal = get_cal(49, 2021);
    save_cal(cal);
    delete_cal(49, 2021);

    cal = get_cal(50, 2021);
    save_cal(cal);

    cal = get_cal(50, 2021);
    save_cal(cal);

    cal = get_cal(51, 2021);
    save_cal(cal);

    printf("\nSize of cal: %ld\n", sizeof(cal));
    prn_file_content();

    prompt_user_intput(regexs);

    return (0);
}