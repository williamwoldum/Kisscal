#include <stdio.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"

int main() {
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

    delete_cal(49, 2021);

    prn_file_content();

    return (0);
}