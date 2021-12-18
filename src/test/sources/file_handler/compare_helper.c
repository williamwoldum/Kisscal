#include "../../headers/file_handler/compare_helper.h"

#include <stdio.h>
#include <string.h>

#include "../../../main/headers/datatypes.h"

int cmp_cals(calendar cal1, calendar cal2) {
    int failed = 0;
    failed += cal1.time != cal2.time;
    failed += cal1.valid != cal2.valid;

    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        failed += cmp_days(cal1.days[i], cal2.days[i]);
    }
    return !failed;
}

int cmp_days(day day1, day day2) {
    int failed = 0;
    failed += day1.time != day2.time;

    int i;
    for (i = 0; i < CONTENT_IN_DAY; i++) {
        failed += cmp_events(day1.events[i], day2.events[i]);
        failed += cmp_assignments(day1.assignments[i], day2.assignments[i]);
    }
    return failed;
}

int cmp_events(event event1, event event2) {
    int failed = 0;
    failed += strcmp(event1.title, event2.title) ? 1 : 0;
    failed += event1.start_time != event2.start_time;
    failed += event1.end_time != event2.end_time;
    failed += event1.valid != event2.valid;
    return failed;
}

int cmp_assignments(assignment assignment1, assignment assignment2) {
    int failed = 0;
    failed += strcmp(assignment1.title, assignment2.title) ? 1 : 0;
    failed += assignment1.deadline != assignment2.deadline;
    failed += assignment1.expected_time != assignment2.expected_time;
    failed += assignment1.elapsed_time != assignment2.elapsed_time;
    failed += assignment1.valid != assignment2.valid;
    return failed;
}