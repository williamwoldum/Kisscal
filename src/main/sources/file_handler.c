#include "../headers/file_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/time_handler.h"

/************************************************************************* Symbolic constants */

#define STORAGE_PATH "../src/main/cal_storage"

/************************************************************************* Static function prototypes */

static int check_cal_has_content(calendar *cal);
static void save_cal(calendar *cal);
static int get_cal_index(time_t cal_time, FILE *file);
static int get_free_index(FILE *file);
static void load_fresh_cal(calendar *cal, time_t cal_time);
static void load_fresh_day(day *day, time_t day_time);
static int get_num_cals(FILE *file);

/************************************************************************* Global functions  */

/**
 * @brief  Makes file if none exist, user can choose to clear file.
 * @note
 * @param  state: clears file if 0
 * @retval None
 */
void prepare_file(int state) {
    if (state == 0) {
        fclose(fopen(STORAGE_PATH, "wb"));
    } else {
        FILE *file = fopen(STORAGE_PATH, "rb");
        if (file == NULL) {
            fclose(fopen(STORAGE_PATH, "wb"));
        } else {
            fclose(file);
        }
    }
}

/**
 * @brief  looks in file and returns stored cal if found else blank cal.
 * @note  cal_time as ID
 * @param  cal_time: monday 12am
 * @retval calendar
 */
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
/**
 * @brief  looks through stored files, if cal found overwrites with invalid cal.
 * @note   cal_time as ID
 * @param  cal_time: monday 12 am
 * @retval None
 */
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

/**
 * @brief  Overwrites seached day with an empty day
 * @note   day_time as index
 * @param  day_time: 12:00 am
 * @retval None
 */
void clear_day(time_t day_time) {
    time_t cal_time = get_cal_time_from_day_time(day_time);
    calendar cal = get_cal(cal_time);
    int dow = get_t_data(day_time, t_dow);
    load_fresh_day(&cal.days[dow], day_time);
    save_cal(&cal);
}

/**
 * @brief  fetches cal and adds event if possible
 * @note
 * @param  *title: points to the title/name of event
 * @param  start_time: holds start time from input
 * @param  end_time:  holds end time from input
 * @retval None
 */
void add_event(char *title, time_t start_time, time_t end_time) {
    if (start_time > end_time) {
        printf("Event '%s' must end after it starts\n", title);
        return;
    }

    if (get_t_data(start_time, t_year) != get_t_data(end_time, t_year) || get_t_data(start_time, t_yday) != get_t_data(end_time, t_yday)) {
        printf("Event '%s' spreads through multiple days\n", title);
        return;
    }

    calendar cal = get_cal(get_cal_time_from_day_time(start_time));
    int dow = get_t_data(start_time, t_dow);

    int is_before, is_after, overlaps = 0;
    int index = 0, search = 1;
    while (search && index < CONTENT_IN_DAY) {
        is_before = (start_time <= cal.days[dow].events[index].start_time && end_time <= cal.days[dow].events[index].start_time);
        is_after = (start_time >= cal.days[dow].events[index].end_time && end_time >= cal.days[dow].events[index].end_time);
        if (!(is_before || is_after)) {
            search = 0;
            overlaps = 1;
        }
        index++;
    }

    if (!overlaps) {
        index = 0, search = 1;
        while (search && index < CONTENT_IN_DAY) {
            if (!cal.days[dow].events[index].valid) {
                sprintf(cal.days[dow].events[index].title, "%s", title);
                cal.days[dow].events[index].start_time = start_time;
                cal.days[dow].events[index].end_time = end_time;
                cal.days[dow].events[index].valid = 1;
                cal.valid = 1;
                search = 0;
            }
            index++;
        }

        save_cal(&cal);

    } else {
        printf("Event '%s' overlaps other events\n", title);
    }
}

/**
 * @brief  fetches cal and make wanted event invalid if possible/found
 * @note
 * @param  start_time: holds start time from input
 * @retval None
 */
void delete_event(time_t start_time) {
    calendar cal = get_cal(get_cal_time_from_day_time(start_time));
    int dow = get_t_data(start_time, t_dow);

    int index = 0, found = 0;
    while (!found && index < CONTENT_IN_DAY) {
        if (cal.days[dow].events[index].start_time == start_time) {
            cal.days[dow].events[index].valid = 0;
            found = 1;
        }
        index++;
    }

    if (!check_cal_has_content(&cal)) {
        ;
        delete_cal(cal.time);
    } else {
        save_cal(&cal);
    }
}
/**
 * @brief  fetches cal and add assignment if possible
 * @note
 * @param  *title: points to the assignment title
 * @param  deadline: holds the deadline from input
 * @param  expected_time: holds the expected time from input
 * @param  elapsed_time: holds the time used on assignment
 * @retval None
 */
void add_assignemnt(char *title, time_t deadline, float expected_time, float elapsed_time) {
    calendar cal = get_cal(get_cal_time_from_day_time(deadline));
    int dow = get_t_data(deadline, t_dow);

    int index = 0, found = 0;
    while (!found && index < CONTENT_IN_DAY) {
        if (!cal.days[dow].assignments[index].valid) {
            sprintf(cal.days[dow].assignments[index].title, "%s", title);
            cal.days[dow].assignments[index].deadline = deadline;
            cal.days[dow].assignments[index].expected_time = expected_time;
            cal.days[dow].assignments[index].elapsed_time = elapsed_time;
            cal.days[dow].assignments[index].valid = 1;
            cal.valid = 1;
            found = 1;
        }
        index++;
    }
    if (found) {
        save_cal(&cal);
    }
}
/**
 * @brief fetches cal and make wanted assignment invalid if possible/found
 * @note
 * @param  deadline: holds the deadline the deadline from input
 * @retval None
 */
void delete_assignment(time_t deadline) {
    calendar cal = get_cal(get_cal_time_from_day_time(deadline));
    int dow = get_t_data(deadline, t_dow);

    int index = 0, found = 0;
    while (!found && index < CONTENT_IN_DAY) {
        if (cal.days[dow].assignments[index].deadline == deadline) {
            cal.days[dow].assignments[index].valid = 0;
            found = 1;
        }
        index++;
    }

    if (!check_cal_has_content(&cal)) {
        ;
        delete_cal(cal.time);
    } else {
        save_cal(&cal);
    }
}

/************************************************************************* Static functions */
/**
 * @brief  Checks for (content event/assignment) in fetched cal
 * @note
 * @param  *cal: points to a specific cal
 * @retval int (bool)
 */
static int check_cal_has_content(calendar *cal) {
    int day_index = 0, content_index = 0, found = 0;

    while (!found && day_index < DAYS_IN_WEEK) {
        while (!found && content_index < CONTENT_IN_DAY) {
            if (cal->days[day_index].events[content_index].valid) {
                found = 1;
            } else if (cal->days[day_index].assignments[content_index].valid) {
                found = 1;
            }
            content_index++;
        }
        day_index++;
        content_index = 0;
    }
    return found;
}
/**
 * @brief  If given cal is valid, save cal at free index
 * @note
 * @param  *cal: points to a specific cal
 * @retval None
 */
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
/**
 * @brief  returns index of cal in file, returns -1 if not found.
 * @note   cal_time as ID
 * @param  cal_time: monday 12 am
 * @param  *file: points to the cal storage file
 * @retval  int
 */
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
/**
 * @brief  returns index with invalid cal or retun idex bottom of file
 * @note
 * @param  *file: points to the cal storage file
 * @retval int
 */
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
/**
 * @brief  loads empty/nonvalid cal
 * @note    cal_time as ID
 * @param  *cal: points to a specific cal
 * @param  cal_time: monday 12 am
 * @retval None
 */
static void load_fresh_cal(calendar *cal, time_t cal_time) {
    cal->time = cal_time;
    cal->valid = 0;

    int dow;
    for (dow = 0; dow < DAYS_IN_WEEK; dow++) {
        time_t day_time = get_day_time_from_cal_time(dow, cal_time);
        load_fresh_day(&cal->days[dow], day_time);
    }
}
/**
 * @brief  loads empty/nonvalid day with no title
 * @note  day_time as ID
 * @param  *day: point to the specific day
 * @param  day_time: 12 am
 * @retval None
 */
static void load_fresh_day(day *day, time_t day_time) {
    day->time = day_time;

    int hod;
    for (hod = 0; hod < CONTENT_IN_DAY; hod++) {
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
/**
 * @brief return number of cals found in file (valid & nonvalid)
 * @note  calc for return val (bytes file/bytes cal)
 * @param  *file: points to the cal storage file
 * @retval int
 */
static int get_num_cals(FILE *file) {
    fseek(file, 0, SEEK_END);
    return ftell(file) / sizeof(calendar);
}

/************************************************************************* Debug functions */
/**
 * @brief prints content in storage file for debug purposes
 * @note
 * @retval None
 */
void prn_file_content(void) {
    FILE *file = fopen(STORAGE_PATH, "rb");
    int length = get_num_cals(file);
    fseek(file, 0, SEEK_SET);

    printf("\nSize (including non-valids): %d\n", length);

    calendar cal;

    int i;
    for (i = 0; i < length; i++) {
        fread(&cal, sizeof(calendar), 1, file);
        if (!cal.valid) {
            printf("(Invalid) ");
        }
        printf("Week: %d, year: %d, time: %ld, \n", get_t_data(cal.time, t_week), get_t_data(cal.time, t_year), cal.time);
    }

    fclose(file);
}