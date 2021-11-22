#include <stdio.h>
#include <time.h>

#include "../header_files/datatypes.h"
#include "../header_files/file_handler.h"

int main() {
    clr_file();

    calendar cal0 = get_cal(45, 2021);
    calendar cal1 = get_cal(44, 2021);
    calendar cal2 = get_cal(43, 2021);
    calendar cal3 = get_cal(44, 2021);
    calendar cal4 = get_cal(47, 2021);
    calendar cal5 = get_cal(45, 2021);
    calendar cal6 = get_cal(44, 2021);
    calendar cal8 = get_cal(49, 2021);
    calendar cal9 = get_cal(46, 2021);
    calendar cal10 = get_cal(45, 2021);
    calendar cal11 = get_cal(44, 2021);
    calendar cal12 = get_cal(43, 2021);
    calendar cal13 = get_cal(44, 2021);
    calendar cal14 = get_cal(47, 2021);
    calendar cal19 = get_cal(47, 2021);
    calendar cal15 = get_cal(45, 2021);
    calendar cal16 = get_cal(44, 2021);
    calendar cal18 = get_cal(49, 2021);
    calendar cal17 = get_cal(46, 2021);

    calendar cal = get_cal(46, 2021);
    cal.days[0].day_of_month = 1;
    save_cal(cal);

    delete_cal(46, 2021);

    prn_file_content();

    return (0);
}