#include "../../headers/ics_handler/convert_cal_to_ics_suite.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../../main/headers/datatypes.h"
#include "../../../main/headers/file_handler.h"
#include "../../../main/headers/ics_handler.h"
#include "../../../main/headers/time_handler.h"
#include "../../headers/CuTest.h"

/************************************************************************* Symbolic constants */

#define WEEK_50_2021 1639393200

/************************************************************************* Tests  */

void convert_cal_to_ics_test_zero_events(CuTest* tc) {
    prepare_file(0);
    time_t cal_time = WEEK_50_2021;
    calendar cal = get_cal(cal_time);
    convert_cal_to_ics(&cal);

    FILE* expected = fopen("./test/assets/export_test_1.ics", "r");
    FILE* actual = fopen("./ics_output.ics", "r");

    if (!actual || !expected) {
        CuFail(tc, "Failed to open ics files");
    }

    char exp_buffer[100], act_buffer[100];
    int succes = 1;
    while (fgets(exp_buffer, sizeof(exp_buffer), expected) && fgets(act_buffer, sizeof(act_buffer), actual)) {
        if (strcmp(exp_buffer, act_buffer)) {
            if (!(strstr(exp_buffer, "UID") && strstr(act_buffer, "UID"))) {
                succes = 0;
            }
        }
    }

    fclose(expected);
    fclose(actual);
    prepare_file(0);

    CuAssertTrue(tc, succes);
}

void convert_cal_to_ics_test_one_event(CuTest* tc) {
    prepare_file(0);
    time_t cal_time = WEEK_50_2021;
    add_event("Party", cal_time, cal_time + SECS_IN_HOUR);
    calendar cal = get_cal(cal_time);
    convert_cal_to_ics(&cal);

    FILE* expected = fopen("./test/assets/export_test_2.ics", "r");
    FILE* actual = fopen("./ics_output.ics", "r");

    if (!actual || !expected) {
        CuFail(tc, "Failed to open ics files");
    }

    char exp_buffer[100], act_buffer[100];
    int succes = 1;
    while (fgets(exp_buffer, sizeof(exp_buffer), expected) && fgets(act_buffer, sizeof(act_buffer), actual)) {
        if (strcmp(exp_buffer, act_buffer)) {
            if (!(strstr(exp_buffer, "UID") && strstr(act_buffer, "UID"))) {
                succes = 0;
            }
        }
    }

    fclose(expected);
    fclose(actual);
    prepare_file(0);

    CuAssertTrue(tc, succes);
}

void convert_cal_to_ics_test_one_event_and_one_assignment(CuTest* tc) {
    prepare_file(0);
    time_t cal_time = WEEK_50_2021;
    add_event("Movie night", cal_time, cal_time + SECS_IN_HOUR);
    add_assignment("Math test", cal_time + SECS_IN_DAY * 2, 7, 3);
    calendar cal = get_cal(cal_time);
    convert_cal_to_ics(&cal);

    FILE* expected = fopen("./test/assets/export_test_3.ics", "r");
    FILE* actual = fopen("./ics_output.ics", "r");

    if (!actual || !expected) {
        CuFail(tc, "Failed to open ics files");
    }

    char exp_buffer[100], act_buffer[100];
    int succes = 1;
    while (fgets(exp_buffer, sizeof(exp_buffer), expected) && fgets(act_buffer, sizeof(act_buffer), actual)) {
        if (strcmp(exp_buffer, act_buffer)) {
            if (!(strstr(exp_buffer, "UID") && strstr(act_buffer, "UID"))) {
                succes = 0;
            }
        }
    }

    fclose(expected);
    fclose(actual);
    prepare_file(0);

    CuAssertTrue(tc, succes);
}

/************************************************************************* Suite  */

CuSuite* get_suite_convert_cal__to_ics_test() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, convert_cal_to_ics_test_zero_events);
    SUITE_ADD_TEST(suite, convert_cal_to_ics_test_one_event);
    SUITE_ADD_TEST(suite, convert_cal_to_ics_test_one_event_and_one_assignment);
    return suite;
}