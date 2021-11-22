#include "../header_files/file_handler.h"

#include <stdio.h>
#include <time.h>

#include "../header_files/datatypes.h"

/************************************************************/

calendar get_cal(int week, int year) {
    FILE *file = fopen("cal_storage", "rb+");

    int index = get_cal_index(week, year, file);

    calendar cal;

    if (index == -1) {
        cal = create_cal(week, year, file);
    } else {
        fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
        fread(&cal, sizeof(calendar), 1, file);
    }

    fclose(file);

    return cal;
}

void save_cal(calendar cal) {
    FILE *file = fopen("cal_storage", "rb+");

    int index = get_cal_index(cal.week, cal.year, file);

    if (index == -1) {
        save_new_cal(cal, file);
    } else {
        fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
        fwrite(&cal, sizeof(calendar), 1, file);
    }

    fclose(file);
}

void delete_cal(int week, int year) {
    FILE *file = fopen("cal_storage", "rb+");

    int index = get_cal_index(week, year, file);

    if (index != -1) {
        calendar cal;
        fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
        fread(&cal, sizeof(calendar), 1, file);
        cal.valid = 0;
        fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
        fwrite(&cal, sizeof(calendar), 1, file);
    }
    fclose(file);
}

/************************************************************/

static int get_cal_index(int week, int year, FILE *file) {
    handle_empty_file(file);

    int index = -1;
    calendar cal = {.week = 0};

    int length;
    fseek(file, 0, SEEK_SET);
    fread(&length, sizeof(int), 1, file);

    int i;
    for (i = 0; i < length; i++) {
        fread(&cal, sizeof(calendar), 1, file);
        if (cal.week == week && cal.year == year && cal.valid)
            index = i;
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

static calendar create_cal(int week, int year, FILE *file) {
    calendar cal = {.year = year, .week = week, .valid = 1};

    fill_cal_with_dates(&cal);
    save_new_cal(cal, file);

    return cal;
}

static void fill_cal_with_dates(calendar *cal) {
    struct tm tm = {.tm_year = cal->year - 1900, .tm_mon = 0, .tm_mday = 4, .tm_hour = 12};
    mktime(&tm);

    int DaysSinceMonday = (tm.tm_wday - 1) % DAYS_IN_WEEK;
    tm.tm_mday += (cal->week - 1) * DAYS_IN_WEEK - DaysSinceMonday;

    int dow;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        mktime(&tm);
        cal->days[dow].month = tm.tm_mon + 1;
        cal->days[dow].day_of_month = tm.tm_mday;
        tm.tm_mday++;
    }
}

static void save_new_cal(calendar cal, FILE *file) {
    int index = inc_cal_counter(file) - 1;
    fseek(file, sizeof(int) + (index * sizeof(calendar)), SEEK_SET);
    fwrite(&cal, sizeof(calendar), 1, file);
}

static int inc_cal_counter(FILE *file) {
    int length;
    fseek(file, 0, SEEK_SET);
    fread(&length, sizeof(int), 1, file);
    length++;
    fseek(file, 0, SEEK_SET);
    fwrite(&length, sizeof(int), 1, file);
    return length;
}

/************************************************************/

void clr_file() {
    FILE *file = fopen("cal_storage", "wb");
    fclose(file);
}

void prn_file_content() {
    FILE *file = fopen("cal_storage", "rb");
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
        }
    }
    fclose(file);
}

void prn_cal_dates(calendar cal) {
    int i;
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        printf("%d/%d ", cal.days[i].day_of_month, cal.days[i].month);
    }
    printf("\n\n");
}