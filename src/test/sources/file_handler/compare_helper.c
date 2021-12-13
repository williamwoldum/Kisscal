#include "../../headers/file_handler/compare_helper.h"

#include <stdio.h>

#include "../../../main/headers/datatypes.h"

int cmp_cals(calendar cal1, calendar cal2) {
    int failed = 0;
    failed += cal1.time == cal2.time;
    failed += cal1.valid != cal2.valid;

    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        failed += !cmp_days(cal1.days[i], cal2.days[i]);
    }

    return !failed;
}

int cmp_days(day day1, day day2) {
    int failed = 0;
    failed += day1.time == day2.time;

    int i;
    for (i = 0; i < HOURS_IN_DAY; i++) {
        failed += !cmp_events(day1.events[i], day2.events[i]);
    }

    return !failed;
}

int cmp_events(event event1, event event2) {
    int failed = 0;
    failed += cmp_titles(event1.title, event2.title) ? 0 : 1;
    failed += event1.start_time != event2.start_time;
    failed += event1.end_time != event2.end_time;
    failed += event1.valid != event2.valid;

    return !failed;
}

int cmp_titles(char title1[], char title2[]) {
    int i = 0, run = 1, failed = 0;
    while (run) {
        failed += (title1[i] != title2[i]);
        if (title1[i] == '\0' || title2[i] == '\0') {
            run = 0;
        }
    }

    return !failed;
}