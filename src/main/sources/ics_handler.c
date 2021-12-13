#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../headers/datatypes.h"
#include "../headers/file_handler.h"

static char *get_dtstart_dtend(event, int);
static int get_uid();
static void create_event(FILE *, calendar, int, int);
static void create_assignment(FILE *, calendar, int, int);
static char *get_deadline(assignment);

void convert_cal_to_ics(calendar *cal)
{
    FILE *cal_file = fopen("./ics_output.ics", "w");
    fprintf(cal_file,
            "BEGIN:VCALENDAR\n"
            "VERSION:2.0\n"
            "PRODID:wtf\n"

    );

    int day;

    for (day = 0; day < DAYS_IN_WEEK; day++)
    {
        int hour;
        for (hour = 0; hour < CONTENT_IN_DAY; hour++)
        {
            if (cal->days[day].events[hour].valid == 1)
            {
                create_event(cal_file, *cal, day, hour);
            }

            if (cal->days[day].assignments[hour].valid == 1)
            {
                create_assignment(cal_file, *cal, day, hour);
            }
        }
    }

    fprintf(cal_file, "END:VCALENDAR\n");
    fclose(cal_file);
    /*cal.days[0].events[0].*/
}

static void create_event(FILE *cal_file, calendar cal, int day, int hour)
{
    char *dtstart = get_dtstart_dtend(cal.days[day].events[hour], 1);

    char *dtend = get_dtstart_dtend(cal.days[day].events[hour], 0);

    fprintf(cal_file,
            "BEGIN:VEVENT\n"
            "UID:%d\n",
            get_uid());

    fprintf(cal_file, "%s\n",
            dtstart);
    free(dtstart);

    fprintf(cal_file, "%s\n",
            dtend);
    free(dtend);

    fprintf(cal_file, "SUMMARY:%s\n", cal.days[day].events[hour].title);

    fprintf(cal_file, "END:VEVENT\n");
}

static void create_assignment(FILE *cal_file, calendar cal, int day, int hour)
{
    char *dtstart = get_deadline(cal.days[day].assignments[hour]);

    char *dtend = get_deadline(cal.days[day].assignments[hour]);

    fprintf(cal_file,
            "BEGIN:VEVENT\n"
            "UID:%d\n",
            get_uid());

    fprintf(cal_file, "DTSTART;%s\n",
            dtstart);
    free(dtstart);

    fprintf(cal_file, "DTEND;%s\n",
            dtend);
    free(dtend);

    fprintf(cal_file, "SUMMARY:%s\n", cal.days[day].assignments[hour].title);

    fprintf(cal_file, "END:VEVENT\n");
}

static char *get_dtstart_dtend(event event, int isStart)
{
    char *final = (char *)malloc(48 * sizeof(char));

    if (isStart)
    {
        time_t start_time = event.start_time;
        struct tm *start = localtime(&start_time);
        sprintf(final, "DTSTART;TZID=Europe/Copenhagen:%02d%02d%02dT%02d%02d%02d", start->tm_year + 1900, start->tm_mon + 1, start->tm_mday, start->tm_hour, start->tm_min, start->tm_sec);
    }
    else
    {
        time_t end_time = event.end_time;
        struct tm *end = localtime(&end_time);
        sprintf(final, "DTEND;TZID=Europe/Copenhagen:%02d%02d%02dT%02d%02d%02d", end->tm_year + 1900, end->tm_mon + 1, end->tm_mday, end->tm_hour, end->tm_min, end->tm_sec);
    }

    return final;
}

static char *get_deadline(assignment assignment)
{
    char *final = (char *)malloc(48 * sizeof(char));
    time_t deadline_time = assignment.deadline;
    struct tm *time = localtime(&deadline_time);

    sprintf(final, "TZID=Europe/Copenhagen:%02d%02d%02dT%02d%02d%02d", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);

    return final;
}

static int get_uid()
{
    int r = rand() % 1000000000;
    return r;
}

void import_ics(calendar cal)
{
    char *path = "/Users/mathiasfrihauge/Documents/UNI/p1projekt/Kisscal/src/Importfiles/ics_output-input.ics";

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File not found");
        return;
    }

    char line[100];

    char c;
    int linecount = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            linecount++;
        }
    }
    fseek(file, 0, SEEK_SET);

    int eventstatus = 0;
    int i = 0;
    time_t start_time = 0;
    time_t end_time = 0;
    char *title;
    for (i = 0; i < linecount; i++)
    {

        fgets(line, 100, file);

        if (strstr(line, "BEGIN:VEVENT"))
        {
            title = (char *)calloc(1, 100);
            eventstatus = 1;
        }
        else if (strstr(line, "END:VEVENT"))
        {
            eventstatus = 0;
            printf("starttime: %ld\n", start_time);
            printf("endtime: %ld\n", end_time);
            add_event(title, start_time, end_time);
            start_time = 0;
            end_time = 0;

            free(title);
            /* add_event(title, start_time, end_time);*/
        }

        if (eventstatus)
        {
            struct tm time;
            time.tm_isdst = 0;
            char buffer[100];
            if (strstr(line, "DTSTART"))
            {

                sscanf(line, "%*[^:]:%s", buffer);
                printf("START %s\n", buffer);
                int year, month;
                sscanf(buffer, "%4d%2d%2dT%2d%2d",
                       &year,
                       &month,
                       &time.tm_mday,
                       &time.tm_hour,
                       &time.tm_min);
                time.tm_year = year - 1900;
                time.tm_mon = month - 1;

                start_time = mktime(&time);
                printf("%ld\n", start_time);
            }
            else if (strstr(line, "DTEND"))
            {

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
                printf("%ld\n", end_time);
            }
            else if (strstr(line, "SUMMARY"))
            {
                char buf[100];
                sscanf(line, "%[^:]:%s", buf, title);

                printf("TITEL %s\n", title);
            }
        }
    }
}
