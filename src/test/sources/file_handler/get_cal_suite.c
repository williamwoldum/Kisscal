#include "../../headers/file_handler/get_cal_suite.h"

#include <stdio.h>

#include "../../../main/headers/datatypes.h"
#include "../../../main/sources/file_handler.c"
#include "../../../main/sources/time_handler.c"
#include "../../headers/CuTest.h"
#include "../../headers/file_handler/compare_helper.h"

#define WEEK_45_2021 1636369200
#define WEEK_53_2015 1451300400
#define WEEK_01_1970 -219600
#define WEEK_01_9999 253371063600

#define WEEK_45_2021_OFFSET 1636369201

void get_cal_test_week_45_2021_fresh(CuTest* tc) {
    prepare_file(0);

    calendar actual = get_cal(WEEK_45_2021);
    calendar expected = {.time = WEEK_45_2021, .valid = 0};

    int dow, i;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = dow * SECS_IN_DAY + WEEK_45_2021;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            expected.days[dow].events[i].title[0] = '\0';
            expected.days[dow].events[i].start_time = -1;
            expected.days[dow].events[i].end_time = -1;
            expected.days[dow].events[i].valid = 0;

            expected.days[dow].assignments[i].title[0] = '\0';
            expected.days[dow].assignments[i].deadline = -1;
            expected.days[dow].assignments[i].expected_time = 0;
            expected.days[dow].assignments[i].elapsed_time = 0;
            expected.days[dow].assignments[i].valid = 0;
        }
    }

    CuAssertTrue(tc, cmp_cals(actual, expected));
}

void get_cal_test_week_53_2015_fresh(CuTest* tc) {
    prepare_file(0);

    calendar actual = get_cal(WEEK_53_2015);
    calendar expected = {.time = WEEK_53_2015, .valid = 0};

    int dow, i;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = dow * SECS_IN_DAY + WEEK_53_2015;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            expected.days[dow].events[i].title[0] = '\0';
            expected.days[dow].events[i].start_time = -1;
            expected.days[dow].events[i].end_time = -1;
            expected.days[dow].events[i].valid = 0;

            expected.days[dow].assignments[i].title[0] = '\0';
            expected.days[dow].assignments[i].deadline = -1;
            expected.days[dow].assignments[i].expected_time = 0;
            expected.days[dow].assignments[i].elapsed_time = 0;
            expected.days[dow].assignments[i].valid = 0;
        }
    }

    CuAssertTrue(tc, cmp_cals(actual, expected));
}

void get_cal_test_week_01_1970_fresh(CuTest* tc) {
    prepare_file(0);

    calendar actual = get_cal(WEEK_01_1970);
    calendar expected = {.time = WEEK_01_1970, .valid = 0};

    int dow, i;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = dow * SECS_IN_DAY + WEEK_01_1970;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            expected.days[dow].events[i].title[0] = '\0';
            expected.days[dow].events[i].start_time = -1;
            expected.days[dow].events[i].end_time = -1;
            expected.days[dow].events[i].valid = 0;

            expected.days[dow].assignments[i].title[0] = '\0';
            expected.days[dow].assignments[i].deadline = -1;
            expected.days[dow].assignments[i].expected_time = 0;
            expected.days[dow].assignments[i].elapsed_time = 0;
            expected.days[dow].assignments[i].valid = 0;
        }
    }

    CuAssertTrue(tc, cmp_cals(actual, expected));
}

void get_cal_test_week_01_9999_fresh(CuTest* tc) {
    prepare_file(0);

    calendar actual = get_cal(WEEK_01_9999);
    calendar expected = {.time = WEEK_01_9999, .valid = 0};

    int dow, i;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = dow * SECS_IN_DAY + WEEK_01_9999;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            expected.days[dow].events[i].title[0] = '\0';
            expected.days[dow].events[i].start_time = -1;
            expected.days[dow].events[i].end_time = -1;
            expected.days[dow].events[i].valid = 0;

            expected.days[dow].assignments[i].title[0] = '\0';
            expected.days[dow].assignments[i].deadline = -1;
            expected.days[dow].assignments[i].expected_time = 0;
            expected.days[dow].assignments[i].elapsed_time = 0;
            expected.days[dow].assignments[i].valid = 0;
        }
    }

    CuAssertTrue(tc, cmp_cals(actual, expected));
}

void get_cal_test_week_45_2021_fresh_offset(CuTest* tc) {
    prepare_file(0);

    calendar actual = get_cal(WEEK_45_2021_OFFSET);
    calendar expected = {.time = WEEK_45_2021_OFFSET, .valid = 0};

    int dow, i;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = dow * SECS_IN_DAY + WEEK_45_2021_OFFSET;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            expected.days[dow].events[i].title[0] = '\0';
            expected.days[dow].events[i].start_time = -1;
            expected.days[dow].events[i].end_time = -1;
            expected.days[dow].events[i].valid = 0;

            expected.days[dow].assignments[i].title[0] = '\0';
            expected.days[dow].assignments[i].deadline = -1;
            expected.days[dow].assignments[i].expected_time = 0;
            expected.days[dow].assignments[i].elapsed_time = 0;
            expected.days[dow].assignments[i].valid = 0;
        }
    }

    CuAssertTrue(tc, !cmp_cals(actual, expected));
}

void get_cal_test_week_45_2021_loaded(CuTest* tc) {
    prepare_file(0);
    char* title = "Test";
    add_event(title, WEEK_45_2021, WEEK_45_2021 + SECS_IN_HOUR);
    calendar actual = get_cal(WEEK_45_2021);

    calendar expected = {.time = WEEK_45_2021, .valid = 1};

    int dow, i;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        expected.days[dow].time = dow * SECS_IN_DAY + WEEK_45_2021;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            expected.days[dow].events[i].title[0] = '\0';
            expected.days[dow].events[i].start_time = -1;
            expected.days[dow].events[i].end_time = -1;
            expected.days[dow].events[i].valid = 0;

            expected.days[dow].assignments[i].title[0] = '\0';
            expected.days[dow].assignments[i].deadline = -1;
            expected.days[dow].assignments[i].expected_time = 0;
            expected.days[dow].assignments[i].elapsed_time = 0;
            expected.days[dow].assignments[i].valid = 0;
        }
    }

    sprintf(expected.days[0].events[0].title, "%s", title);
    expected.days[0].events[0].start_time = WEEK_45_2021;
    expected.days[0].events[0].end_time = WEEK_45_2021 + SECS_IN_HOUR;
    expected.days[0].events[0].valid = 1;

    CuAssertTrue(tc, cmp_cals(actual, expected));
    prepare_file(0);
}

CuSuite* get_suite_get_cal_test() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, get_cal_test_week_45_2021_fresh);
    SUITE_ADD_TEST(suite, get_cal_test_week_53_2015_fresh);
    SUITE_ADD_TEST(suite, get_cal_test_week_01_1970_fresh);
    SUITE_ADD_TEST(suite, get_cal_test_week_01_9999_fresh);
    SUITE_ADD_TEST(suite, get_cal_test_week_45_2021_fresh_offset);
    SUITE_ADD_TEST(suite, get_cal_test_week_45_2021_loaded);
    return suite;
}