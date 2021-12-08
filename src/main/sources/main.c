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
#include "../headers/time_handler.h"

int main()
{
    srand(time(NULL));

    setup_regex();

    clr_file();
<<<<<<< HEAD
    calendar cal = get_cal(47, 2021);

    cal = get_cal(49, 2021);

    cal = get_cal(50, 2021);

    cal = get_cal(50, 2021);

    /*ICS TESTING*/
    convert_cal_to_ics(get_cal(45, 2021));
    import_ics("test");
    /*ICS TESTING*/

    cal = get_cal(51, 2021);

    cal = get_cal(51, -2021);

    printf("\nSize of cal: %ld\n", sizeof(cal));
    prn_file_content();

    prompt_user_input(regexs);
=======
    /*Courser segfault hvis over clr_file()*/
    setup_renderer();
    prn_cal();

    int run = 1;
    while (run) {
        run = prompt_user_input();
        printf("%d %d %ld\n", get_t_data(current_cal.time, t_week), get_t_data(current_cal.time, t_year), current_cal.time);
    }
>>>>>>> 1a30afd319a856d1e63c975bd4f47ab41ebf4d27

    return (0);
}