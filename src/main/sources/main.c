#include <stdio.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"

int main() {
    clr_file();

    calendar cal = get_cal(46, 2021);
    cal.days[0].dom = 1;
    save_cal(cal);

    cal = get_cal(47, 2021);
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

    return (0);
}