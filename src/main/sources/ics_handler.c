#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"
#include "../headers/time_handler.h"

/************************************************************************* Symbolic constants */

#define ICS_OUTPUT_PATH "./ics_output.ics"
#define ICS_INPUT_DIR_PATH "./Importfiles"

/************************************************************************* Static function prototypes */

static void event_to_ics(FILE *ics_file, event *event);
static void assignment_to_ics(FILE *ics_file, assignment *assignment);
static int get_uid(void);

/************************************************************************* Global functions  */

/**
 * @brief  Outputs a ics file with data loaded from given calendar
 * @note
 * @param  *cal: Calendar to be converted to ics
 * @retval None
 */
void convert_cal_to_ics(calendar *cal) {
    FILE *ics_file = fopen(ICS_OUTPUT_PATH, "w");
    fprintf(ics_file,
            "BEGIN:VCALENDAR\n"
            "VERSION:2.0\n"
            "PRODID:wtf\n");

    int day;
    for (day = 0; day < DAYS_IN_WEEK; day++) {
        int i;
        for (i = 0; i < CONTENT_IN_DAY; i++) {
            if (cal->days[day].events[i].valid == 1) {
                event_to_ics(ics_file, &cal->days[day].events[i]);
            }

            if (cal->days[day].assignments[i].valid == 1) {
                assignment_to_ics(ics_file, &cal->days[day].assignments[i]);
            }
        }
    }

    fprintf(ics_file, "END:VCALENDAR\n");
    fclose(ics_file);
}

/**
 * @brief  Reads ics files and saves data into the calendar storage file
 * @note
 * @retval None
 */
void import_ics(void) {
    DIR *folder;
    struct dirent *dir;
    char *name;
    folder = opendir(ICS_INPUT_DIR_PATH);

    while ((dir = readdir(folder)) != NULL) {
        printf("name %s\n", dir->d_name);
        name = dir->d_name;
    }

    char path[300];

    sprintf(path, "./Importfiles/%s", name);
    printf("path %s\n", path);
    closedir(folder);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File not found");
        return;
    }

    char line[100];

    char c;
    int linecount = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            linecount++;
        }
    }
    fseek(file, 0, SEEK_SET);

    int eventstatus = 0;
    int i = 0;
    time_t start_time = 0;
    time_t end_time = 0;
    char *title;
    struct tm time;
    int year1 = 0, month1 = 0;

    /*lorte kode til at sætte en standard værdi åbenbart*/
    sscanf("20211005T123410", "%4d%2d%2dT%2d%2d",
           &year1,
           &month1,
           &time.tm_mday,
           &time.tm_hour,
           &time.tm_min);
    time.tm_year = year1 - 1900;
    time.tm_mon = month1 - 1;
    start_time = mktime(&time);
    /*------------------------------------*/

    time.tm_isdst = 0;

    for (i = 0; i < linecount; i++) {
        fgets(line, 100, file);

        if (strstr(line, "BEGIN:VEVENT")) {
            title = (char *)calloc(1, 100);
            eventstatus = 1;
        } else if (strstr(line, "END:VEVENT")) {
            eventstatus = 0;
            printf("starttime: %ld\n", start_time);
            printf("endtime: %ld\n", end_time);
            add_event(title, start_time, end_time);
            start_time = 0;
            end_time = 0;

            free(title);
            /* add_event(title, start_time, end_time);*/
        }

        if (eventstatus) {
            /*time.tm_isdst = 0;*/

            char *buffer = (char *)calloc(1, 100);

            if (strstr(line, "DTSTART")) {
                sscanf(line, "%*[^:]:%s", buffer);

                int year = 0, month = 0;
                sscanf(buffer, "%4d%2d%2dT%2d%2d",
                       &year,
                       &month,
                       &time.tm_mday,
                       &time.tm_hour,
                       &time.tm_min);
                time.tm_year = year - 1900;
                time.tm_mon = month - 1;
                printf("START %s\n", buffer);
                start_time = mktime(&time);
                printf("actualtime: %ld\n", start_time);
            } else if (strstr(line, "DTEND")) {
                sscanf(line, "%*[^:]:%s", buffer);
                printf("SLUT %s\n", buffer);
                int year, month;
                sscanf(buffer, "%4d%2d%2dT%2d%2d",
                       &year,
                       &month,
                       &time.tm_mday,
                       &time.tm_hour,
                       &time.tm_min);
                time.tm_year = year - 1900;
                time.tm_mon = month - 1;

                end_time = mktime(&time);
                printf("actualtime: %ld\n", end_time);
            } else if (strstr(line, "SUMMARY")) {
                char buf[100];
                sscanf(line, "%[^:]:%s", buf, title);

                printf("TITEL %s\n", title);
            }
        }
    }
}

/************************************************************************* Static functions */

/**
 * @brief  Converts calendar event to ics and writes it to ics file
 * @note
 * @param  *ics_file: Points to ics output file
 * @param  *event: Event to be converted
 * @retval None
 */
static void event_to_ics(FILE *ics_file, event *event) {
    char dtstart[48];
    char dtend[48];
    load_epoch_to_utc(dtstart, "DTSTART;TZID=Europe/Copenhagen:", event->start_time);
    load_epoch_to_utc(dtstart, "DTEND;TZID=Europe/Copenhagen:", event->end_time);

    fprintf(ics_file, "BEGIN:VEVENT\n");
    fprintf(ics_file, "UID:%d\n", get_uid());
    fprintf(ics_file, "%s\n", dtstart);
    fprintf(ics_file, "%s\n", dtend);
    fprintf(ics_file, "SUMMARY:%s\n", event->title);
    fprintf(ics_file, "END:VEVENT\n");
}

/**
 * @brief  Converts calendar assignment to ics and writes it to ics file
 * @note
 * @param  *ics_file: Points to ics output file
 * @param  *event: Assignment to be converted
 * @retval None
 */
static void assignment_to_ics(FILE *ics_file, assignment *assignment) {
    char dtstart_dtend[48];
    load_epoch_to_utc(dtstart_dtend, "TZID=Europe/Copenhagen:", assignment->deadline);

    fprintf(ics_file, "BEGIN:VEVENT\n");
    fprintf(ics_file, "UID:%d\n", get_uid());
    fprintf(ics_file, "DTSTART;%s\n", dtstart_dtend);
    fprintf(ics_file, "DTEND;%s\n", dtstart_dtend);
    fprintf(ics_file, "SUMMARY:%s\n", assignment->title);
    fprintf(ics_file, "END:VEVENT\n");
}

/**
 * @brief  Generates pseudo random id for ics event
 * @note
 * @retval int
 */
static int get_uid(void) {
    int r = rand() % 1000000000;
    return r;
}