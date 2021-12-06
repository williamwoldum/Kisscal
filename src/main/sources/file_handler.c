#include "../headers/file_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"

/************************************************************************* Symbolic constants */

#define STORAGE_PATH "../src/main/cal_storage"

/************************************************************************* Static function prototypes */

static void save_cal(calendar *cal);
static int get_cal_index(int week, int year, FILE *file);
static int get_free_index(int week, int year, FILE *file);
static void load_fresh_cal(calendar *cal, int week, int year);
static void load_fresh_day(day *day, int dow, int week, int year);
static void fill_day_with_date(day *day, int dow, int week, int year);
static int get_num_cals(FILE *file);
void constrain_week(int *week, int *year);

/************************************************************************* Global functions  */

calendar get_cal(int week, int year) {
    constrain_week(&week, &year);

    FILE *file = fopen(STORAGE_PATH, "rb+");

    int index = get_cal_index(week, year, file);

    calendar cal;

    if (index == -1) {
        load_fresh_cal(&cal, week, year);
    } else {
        fseek(file, index * sizeof(calendar), SEEK_SET);
        fread(&cal, sizeof(calendar), 1, file);
    }

    fclose(file);

    return cal;
}

void delete_cal(int week, int year) {
    FILE *file = fopen(STORAGE_PATH, "rb+");

    int index = get_cal_index(week, year, file);

    if (index != -1) {
        calendar cal = {.valid = 0};
        fseek(file, index * sizeof(calendar), SEEK_SET);
        fwrite(&cal, sizeof(calendar), 1, file);
    }
    fclose(file);
}

void clear_day(int dow, int week, int year) {
    calendar cal = get_cal(week, year);
    load_fresh_day(&cal.days[dow], dow, week, year);
    save_cal(&cal);
}

void add_event(int dow, int week, int year, char *title, time_t start_time, time_t end_time) {
    if (start_time >= end_time) {
        printf("Event '%s' must end after it starts", title);
    }

    calendar cal = get_cal(week, year);

    int i, is_before, is_after, overlaps = 0;
    int index = 0, search = 1;
    while (search && index < HOURS_IN_DAY * 2) {
        is_before = (start_time <= cal.days[dow].events[index].start_time && end_time <= cal.days[dow].events[index].start_time);
        is_after = (start_time >= cal.days[dow].events[index].end_time && end_time >= cal.days[dow].events[index].end_time);
        if (!(is_before || is_after)) {
            search = 0;
            overlaps = 1;
        }
    }

    if (!overlaps) {
        index = 0, search = 1;
        while (search && index < HOURS_IN_DAY * 2) {
            if (!cal.days[dow].events[index].valid) {
                for (i = 0; i < TITLE_LENGTH; i++) {
                    cal.days[dow].events[index].title[i] = title[i];
                }
                cal.days[dow].events[index].start_time = start_time;
                cal.days[dow].events[index].end_time = end_time;
                cal.days[dow].events[index].valid = 1;
                search = 0;
            }
        }
    } else {
        printf("Event '%s' overlaps other events", title);
    }
}
/************************************************************************* Static functions */

static void save_cal(calendar *cal) {
    if (cal->valid) {
        FILE *file = fopen(STORAGE_PATH, "rb+");

        int index = get_cal_index(cal->week, cal->year, file);

        if (index == -1) {
            index = get_free_index(cal->week, cal->year, file);
        }

        fseek(file, index * sizeof(calendar), SEEK_SET);
        fwrite(&cal, sizeof(calendar), 1, file);

        fclose(file);
    }
}

static int get_cal_index(int week, int year, FILE *file) {
    int length = get_num_cals(file);

    fseek(file, 0, SEEK_SET);
    calendar cal;
    int index = 0, found = 0;
    while (!found && index < length) {
        fread(&cal, sizeof(calendar), 1, file);
        if (cal.week == week && cal.year == year && cal.valid) {
            found = 1;
        } else {
            index++;
        }
    }

    return index == length ? -1 : index;
}

static int get_free_index(int week, int year, FILE *file) {
    int length = get_num_cals(file);

    fseek(file, 0, SEEK_SET);
    calendar cal;
    int index = 0, found = 0;
    while (!found && index < length) {
        fread(&cal, sizeof(calendar), 1, file);
        if (!cal.valid) {
            found = 1;
        } else {
            index++;
        }
    }

    return index;
}

static void load_fresh_cal(calendar *cal, int week, int year) {
    cal->year = year;
    cal->week = week;
    cal->valid = 0;

    int dow;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        load_fresh_day(&cal->days[dow], dow, week, year);
    }
}

static void load_fresh_day(day *day, int dow, int week, int year) {
    fill_day_with_date(day, dow, week, year);

    int hod;
    for (hod = 0; hod < HOURS_IN_DAY * 2; hod++) {
        day->events[hod].title[0] = '\0';
        day->events[hod].start_time = -1;
        day->events[hod].end_time = -1;
        day->events[hod].valid = 0;

        day->assignments[hod].title[0] = '\0';
        day->assignments[hod].deadline = -1;
        day->assignments[hod].expected_time = 0;
        day->assignments[hod].elapsed_time = 0;
        day->assignments[hod].valid = 0;
    }
}

static void fill_day_with_date(day *day, int dow, int week, int year) {
    struct tm tm = {.tm_year = year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);

    int DaysSinceMonday = (tm.tm_wday - 1) % DAYS_IN_WEEK;
    tm.tm_mday += (week - 1) * DAYS_IN_WEEK - DaysSinceMonday + dow;

    mktime(&tm);
    day->year = tm.tm_year + 1900;
    day->month = tm.tm_mon + 1;
    day->dom = tm.tm_mday;
}

static int get_num_cals(FILE *file) {
    fseek(file, 0, SEEK_END);
    return ftell(file) / sizeof(calendar);
}

void constrain_week(int *week, int *year) {
    if (*year < 1900) {
        *year = 1900;
    } else if (*year > 9999) {
        *year = 9999;
    }

    struct tm tm = {.tm_year = *year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);
    int DaysSinceMonday = (tm.tm_wday - 1) % DAYS_IN_WEEK;
    tm.tm_mday += (*week - 1) * DAYS_IN_WEEK - DaysSinceMonday;
    mktime(&tm);

    *year = tm.tm_year + 1900;
    char week_number_str[3];
    strftime(week_number_str, 3, "%W", &tm);
    *week = atoi(week_number_str);
}

/************************************************************************* Debug functions */

void clr_file() {
    fclose(fopen(STORAGE_PATH, "wb"));
}

void prn_file_content() {
    FILE *file = fopen(STORAGE_PATH, "rb");
    int length = get_num_cals(file);

    fseek(file, 0, SEEK_SET);

    printf("\nSize (including non-valids): %d\n", length);

    calendar cal;
    int i;
    for (i = 0; i < length; i++) {
        fread(&cal, sizeof(calendar), 1, file);
        if (cal.valid) {
            printf("Week: %d, year: %d\nDates: ", cal.week, cal.year);
            prn_cal_dates(&cal);
        } else {
            printf("(Invalid) Week: %d, year: %d\nDates: ", cal.week, cal.year);
            prn_cal_dates(&cal);
        }
    }
    fclose(file);
}

void prn_cal_dates(calendar *cal) {
    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%d/%d ", cal->days[i].dom, cal->days[i].month);
    }
    printf("\n\n");
}