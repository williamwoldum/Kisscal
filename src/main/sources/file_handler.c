#include "../headers/file_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/time_handler.h"

/************************************************************************* Symbolic constants */

#define STORAGE_PATH "../src/main/cal_storage"

/************************************************************************* Static function prototypes */

static void save_cal(calendar *cal);
static int get_cal_index(time_t cal_time, FILE *file);
static int get_free_index(FILE *file);
static void load_fresh_cal(calendar *cal, time_t cal_time);
static void load_fresh_day(day *day, time_t day_time);
static int get_num_cals(FILE *file);

/************************************************************************* Global functions  */

calendar get_cal(time_t cal_time) {
    FILE *file = fopen(STORAGE_PATH, "rb");

    int index = get_cal_index(cal_time, file);

    calendar cal;

    if (index == -1) {
        load_fresh_cal(&cal, cal_time);
    } else {
        fseek(file, index * sizeof(calendar), SEEK_SET);
        fread(&cal, sizeof(calendar), 1, file);
    }

    fclose(file);

    return cal;
}

void delete_cal(time_t cal_time) {
    FILE *file = fopen(STORAGE_PATH, "rb+");

    int index = get_cal_index(cal_time, file);

    if (index != -1) {
        calendar cal = {.valid = 0};
        fseek(file, index * sizeof(calendar), SEEK_SET);
        fwrite(&cal, sizeof(calendar), 1, file);
    }
    fclose(file);
}

void clear_day(time_t day_time) {
    time_t cal_time = get_cal_time_from_day_time(day_time);
    calendar cal = get_cal(cal_time);
    int dow = get_t_data(day_time, t_dow);
    load_fresh_day(&cal.days[dow], day_time);
    save_cal(&cal);
}

void add_event(char *title, time_t start_time, time_t end_time) {
    if (start_time >= end_time) {
        printf("Event '%s' must end after it starts", title);
        return;
    }

    calendar cal = get_cal(get_cal_time_from_day_time(start_time));

    int dow = get_t_data(start_time, t_dow);

    int is_before, is_after, overlaps = 0;
    int index = 0, search = 1;
    while (search && index++ < HOURS_IN_DAY * 2) {
        is_before = (start_time <= cal.days[dow].events[index].start_time && end_time <= cal.days[dow].events[index].start_time);
        is_after = (start_time >= cal.days[dow].events[index].end_time && end_time >= cal.days[dow].events[index].end_time);
        if (!(is_before || is_after)) {
            search = 0;
            overlaps = 1;
        }
    }

    if (!overlaps) {
        index = 0, search = 1;
        while (search && index++ < HOURS_IN_DAY * 2) {
            if (!cal.days[dow].events[index].valid) {
                sprintf(cal.days[dow].events[index].title, "%s", title);
                cal.days[dow].events[index].start_time = start_time;
                cal.days[dow].events[index].end_time = end_time;
                cal.days[dow].events[index].valid = 1;
                cal.valid = 1;
                search = 0;
            }
        }

        save_cal(&cal);
        prn_file_content();

        prn_day_content(cal.time);

    } else {
        printf("Event '%s' overlaps other events", title);
    }
}
/************************************************************************* Static functions */

static void save_cal(calendar *cal) {
    if (cal->valid) {
        FILE *file = fopen(STORAGE_PATH, "rb+");

        int index = get_cal_index(cal->time, file);

        if (index == -1) {
            index = get_free_index(file);
        }

        fseek(file, index * sizeof(calendar), SEEK_SET);
        fwrite(cal, sizeof(calendar), 1, file);

        fclose(file);
    }
}

static int get_cal_index(time_t cal_time, FILE *file) {
    int length = get_num_cals(file);

    fseek(file, 0, SEEK_SET);
    calendar cal;
    int index = 0, found = 0;
    while (!found && index < length) {
        fread(&cal, sizeof(calendar), 1, file);
        if (cal.time == cal_time && cal.valid) {
            found = 1;
        } else {
            index++;
        }
    }

    return index == length ? -1 : index;
}

static int get_free_index(FILE *file) {
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

static void load_fresh_cal(calendar *cal, time_t cal_time) {
    cal->time = cal_time;
    cal->valid = 0;

    int dow;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        time_t day_time = get_day_time_from_cal_time(dow, cal_time);
        load_fresh_day(&cal->days[dow], day_time);
    }
}

static void load_fresh_day(day *day, time_t day_time) {
    day->time = day_time;

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

static int get_num_cals(FILE *file) {
    fseek(file, 0, SEEK_END);
    return ftell(file) / sizeof(calendar);
}

/************************************************************************* Debug functions */

void clr_file(void) {
    fclose(fopen(STORAGE_PATH, "wb"));
}

void prn_file_content(void) {
    FILE *file = fopen(STORAGE_PATH, "rb");
    int length = get_num_cals(file);

    printf("\nSize (including non-valids): %d\n", length);

    fseek(file, 0, SEEK_SET);

    calendar cal;

    int i;
    for (i = 0; i < length; i++) {
        fread(&cal, sizeof(calendar), 1, file);
        if (!cal.valid) {
            printf("(Invalid) ");
        }
        printf("Time: %ld, Week: %d, year: %d\n", cal.time, get_t_data(cal.time, t_week), get_t_data(cal.time, t_year));
    }

    fclose(file);
}

void prn_day_content(time_t cal_time) {
    calendar cal = get_cal(cal_time);
    int i, j;

    for (i = 0; i < DAYS_IN_WEEK; i++) {
        for (j = 0; j < HOURS_IN_DAY * 2; j++) {
            event event = cal.days[i].events[j];
            if (event.valid) {
                printf("%d: title %s start: %ld end: %ld\n", j, event.title, event.start_time, event.end_time);
            }
        }
    }
}
