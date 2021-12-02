#include "../headers/file_handler.h"

#include <stdio.h>
#include <time.h>

#include "../headers/datatypes.h"

/************************************************************************* Symbolic constants */

#define STORAGE_PATH "../src/main/cal_storage"

/************************************************************************* Static function prototypes */

static int get_cal_index(int week, int year, FILE *file);
static int get_free_index(int week, int year, FILE *file);
static void handle_empty_file(FILE *file);
static calendar get_fresh_cal(int week, int year);
static day get_fresh_day(int dow, int week, int year);
static void fill_day_with_date(day *day, int dow, int week, int year);
static int inc_cal_counter(FILE *file);
static int get_cal_counter(FILE *file);

/************************************************************************* Global functions  */

calendar get_cal(int week, int year) {
    FILE *file = fopen(STORAGE_PATH, "rb+");

    int index = get_cal_index(week, year, file);

    calendar cal;

    if (index == -1) {
        cal = get_fresh_cal(week, year);
    } else {
        fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
        fread(&cal, sizeof(calendar), 1, file);
    }

    fclose(file);

    return cal;
}

void save_cal(calendar cal) {
    FILE *file = fopen(STORAGE_PATH, "rb+");

    int index = get_cal_index(cal.week, cal.year, file);

    if (index == -1) {
        index = get_free_index(cal.week, cal.year, file);
        if (index == get_cal_counter(file)) {
            inc_cal_counter(file);
        }
    }

    fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
    fwrite(&cal, sizeof(calendar), 1, file);

    fclose(file);
}

void delete_cal(int week, int year) {
    FILE *file = fopen(STORAGE_PATH, "rb+");

    int index = get_cal_index(week, year, file);

    if (index != -1) {
        calendar cal = {.valid = 0};
        fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
        fwrite(&cal, sizeof(calendar), 1, file);
    }
    fclose(file);
}

void clear_day(int dow, int week, int year) {
    calendar cal = get_cal(week, year);
    cal.days[dow] = get_fresh_day(dow, week, year);
    save_cal(cal);
}

void add_event(int dow, int week, int year, char *title, time_t start_time, time_t end_time) {
    calendar cal = get_cal(week, year);

    int i, is_before, is_after, overlaps = 0;
    for (i = 0; i < HOURS_IN_DAY * 2; i++) {
        is_before = (start_time <= cal.days[dow].events[i].start_time && end_time <= cal.days[dow].events[i].start_time);
        is_after = (start_time >= cal.days[dow].events[i].end_time && end_time >= cal.days[dow].events[i].end_time);
        if (!(is_before || is_after)) {
            overlaps = 1;
        }
    }

    if (!overlaps) {
    }
}

/************************************************************************* Static functions */

static int get_cal_index(int week, int year, FILE *file) {
    handle_empty_file(file);

    int length = get_cal_counter(file);

    fseek(file, sizeof(int), SEEK_SET);
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
    handle_empty_file(file);

    int length = get_cal_counter(file);

    fseek(file, sizeof(int), SEEK_SET);
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

static void handle_empty_file(FILE *file) {
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);

    if (file_size == 0) {
        int length = 0;
        fseek(file, 0, SEEK_SET);
        fwrite(&length, sizeof(int), 1, file);
    }
}

static calendar get_fresh_cal(int week, int year) {
    calendar cal = {.year = year, .week = week, .valid = 0};

    int dow;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        cal.days[dow] = get_fresh_day(dow, week, year);
    }

    return cal;
}

static day get_fresh_day(int dow, int week, int year) {
    day day;

    fill_day_with_date(&day, dow, week, year);

    int hod;
    for (hod = 0; hod < HOURS_IN_DAY * 2; hod++) {
        day.events[hod].title[0] = '\0';
        day.events[hod].start_time = -1;
        day.events[hod].end_time = -1;
        day.events[hod].valid = 0;

        day.assignments[hod].title[0] = '\0';
        day.assignments[hod].deadline = -1;
        day.assignments[hod].expected_time = 0;
        day.assignments[hod].elapsed_time = 0;
        day.assignments[hod].valid = 0;
    }
    return day;
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

static int inc_cal_counter(FILE *file) {
    int length = get_cal_counter(file);
    length++;
    fseek(file, 0, SEEK_SET);
    fwrite(&length, sizeof(int), 1, file);
    return length;
}

static int get_cal_counter(FILE *file) {
    int length;
    fseek(file, 0, SEEK_SET);
    fread(&length, sizeof(int), 1, file);
    return length;
}

/************************************************************************* Debug functions */

void clr_file() {
    fclose(fopen(STORAGE_PATH, "wb"));
}

void prn_file_content() {
    FILE *file = fopen(STORAGE_PATH, "rb");
    int length;

    fseek(file, 0, SEEK_SET);
    fread(&length, sizeof(int), 1, file);

    printf("\nSize (including non-valids): %d\n", length);

    calendar cal;
    int i;
    for (i = 0; i < length; i++) {
        fread(&cal, sizeof(calendar), 1, file);
        if (cal.valid) {
            printf("Week: %d, year: %d\nDates: ", cal.week, cal.year);
            prn_cal_dates(cal);
        } else {
            printf("(Invalid) Week: %d, year: %d\nDates: ", cal.week, cal.year);
            prn_cal_dates(cal);
        }
    }
    fclose(file);
}

void prn_cal_dates(calendar cal) {
    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%d/%d ", cal.days[i].dom, cal.days[i].month);
    }
    printf("\n\n");
}