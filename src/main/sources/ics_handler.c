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
#define DT_BUFFER_SIZE 100
#define PATH_BUFFER_SIZE 300
#define LINE_BUFFER_SIZE 100

/************************************************************************* Static function prototypes */

static void event_to_ics(FILE *ics_file, event *event);
static void assignment_to_ics(FILE *ics_file, assignment *assignment);
static int get_uid(void);

/************************************************************************* Global functions  */

/**
 * @brief  Outputs an ics file with data loaded from given calendar
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

    int day, i;
    for (day = 0; day < DAYS_IN_WEEK; day++) {
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
    folder = opendir(ICS_INPUT_DIR_PATH);

    struct dirent *dir;
    char *name;

    while ((dir = readdir(folder)) != NULL) {
        name = dir->d_name;
    }
    char path[PATH_BUFFER_SIZE];

    sprintf(path, "./Importfiles/%s", name);
    closedir(folder);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File not found");
        return;
    }

    char c;
    int linecount = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            linecount++;
        }
    }
    fseek(file, 0, SEEK_SET);

    int eventstatus = 0;
    time_t start_time, end_time;
    char *title;
    char line[LINE_BUFFER_SIZE];

    int i = 0;
    for (i = 0; i < linecount; i++) {
        fgets(line, LINE_BUFFER_SIZE, file);

        if (strstr(line, "BEGIN:VEVENT")) {
            title = (char *)calloc(1, LINE_BUFFER_SIZE);
            eventstatus = 1;
        } else if (strstr(line, "END:VEVENT")) {
            add_event(title, start_time, end_time);
            eventstatus = 0;
            free(title);
        }

        if (eventstatus) {
            char buffer[LINE_BUFFER_SIZE];

            if (strstr(line, "DTSTART")) {
                sscanf(line, "%*[^:]:%s", buffer);
                start_time = utc_to_epoch(buffer);
            } else if (strstr(line, "DTEND")) {
                sscanf(line, "%*[^:]:%s", buffer);
                end_time = utc_to_epoch(buffer);
            } else if (strstr(line, "SUMMARY")) {
                char buf[LINE_BUFFER_SIZE];
                sscanf(line, "%[^:]:%s", buf, title);
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
    char dtstart[DT_BUFFER_SIZE];
    char dtend[DT_BUFFER_SIZE];
    load_epoch_to_utc(dtstart, "DTSTART;TZID=Europe/Copenhagen:", event->start_time);
    load_epoch_to_utc(dtend, "DTEND;TZID=Europe/Copenhagen:", event->end_time);

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
    char dtstart_dtend[DT_BUFFER_SIZE];
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