#include <stdio.h>

#include "../headers/CuTest.h"
#include "../headers/include_suites.h"

CuSuite* StrUtilGetSuite();
void Run_All_Tests(void);

int main(void) {
    Run_All_Tests();
    return 0;
}

void Run_All_Tests(void) {
    CuString* output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    /* FIle handler suites */
    CuSuiteAddSuite(suite, (CuSuite*)get_suite_get_cal_test());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}
