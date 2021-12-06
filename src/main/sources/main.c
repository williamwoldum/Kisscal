#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/ics_handler.h"

int main()
{
    srand(time(NULL));
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

    /*prn_file_content();*/

    convert_cal_to_ics(get_cal(45, 2021));

    return (0);
}