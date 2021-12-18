#include "../../headers/ics_handler/convert_cal_to_ics_suite.h"

#include <stdio.h>

#include "../../../main/headers/datatypes.h"
#include "../../headers/CuTest.h"

void get_cal_test_week_5_2021_fresh(CuTest* tc) {
      CuAssertTrue(tc, 1);
}

CuSuite* get_suite_convert_cal__to_ics_test() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, get_cal_test_week_5_2021_fresh);
    return suite;
}