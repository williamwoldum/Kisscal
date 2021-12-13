#include <stdlib.h>
#include <time.h>

#include "../headers/cal_renderer.h"
#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/input_handler.h"
#include "../headers/regex_handler.h"
#include "../headers/time_handler.h"

/**
 * @brief  Run the code from all src
 * @note   NONE
 * @retval int
 */
int main() {
    srand(time(NULL));

    setup_regex();
    prepare_file(0);

    time_t cal_time = get_cal_time_from_day_time(time(NULL));
    calendar current_cal = get_cal(cal_time);

    prn_cal(&current_cal);

    int run = 1;
    while (run) {
        run = prompt_user_input(&current_cal);
    }

    return EXIT_SUCCESS;
}