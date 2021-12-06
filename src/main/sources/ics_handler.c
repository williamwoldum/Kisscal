#include "../headers/datatypes.h"
#include "../headers/file_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char *get_DTSTAMP(calendar, int, int);
char *get_dtstart_dtend(event, int);
int get_uid();

void convert_cal_to_ics(calendar cal)
{

    /*opretter test element*/
    cal.days[1].events[0].duration_start = time(NULL);
    cal.days[1].events[0].duration_end = time(NULL) + 3600 * 24;
    cal.days[1].events[0].valid = 1;

    cal.days[1].events[1].duration_start = time(NULL);
    cal.days[1].events[1].duration_end = time(NULL) + 3600 * 2;
    cal.days[1].events[1].valid = 1;

    /*strncpy(cal.days[1].events[0].title, "fuck", 16);*/
    strcpy(cal.days[1].events[0].title, "fuck");

    FILE *cal_file = fopen("./ics_output.ics", "w");
    fprintf(cal_file, "BEGIN:VCALENDAR\n"
                      "VERSION:2.0\n"
                      "PRODID:wtf\n"

    );

    int i;

    for (i = 0; i < 7; i++)
    {
        int j;
        for (j = 0; j < 48; j++)
        {
            if (cal.days[i].events[j].valid == 1)
            {
                char *dtstamp = get_DTSTAMP(cal, i, j);

                char *dtstart = get_dtstart_dtend(cal.days[i].events[j], 1);

                char *dtend = get_dtstart_dtend(cal.days[i].events[j], 0);

                fprintf(cal_file, "BEGIN:VEVENT\n"
                                  "UID:%d\n"
                                  "%s\n",
                        get_uid(), dtstamp);

                free(dtstamp);

                fprintf(cal_file, "%s\n",
                        dtstart);
                free(dtstart);

                fprintf(cal_file, "%s\n",
                        dtend);
                free(dtend);

                fprintf(cal_file, "SUMMARY:%s\n", cal.days[1].events[0].title);

                fprintf(cal_file, "END:VEVENT\n");
            }
        }
    }

    fprintf(cal_file, "END:VCALENDAR\n");
    fclose(cal_file);
    /*cal.days[0].events[0].*/
}

char *get_DTSTAMP(calendar cal, int day, int event)
{
    time_t start_time = cal.days[day].events[event].duration_start;

    int year = cal.year;
    int month = cal.days->month;
    int date = cal.days->dom;

    struct tm *start = localtime(&start_time);

    char *final = (char *)malloc(48 * sizeof(char));

    sprintf(final, "DTSTAMP:%02d%02d%02dT%02d%02d%02dZ", year, month, date, start->tm_hour, start->tm_min, start->tm_sec);

    return final;
}

char *get_dtstart_dtend(event event, int isStart)
{

    char *final = (char *)malloc(48 * sizeof(char));

    if (isStart)
    {
        time_t start_time = event.duration_start;
        struct tm *start = localtime(&start_time);
        sprintf(final, "DTSTART;TZID=Europe/Copenhagen:%02d%02d%02dT%02d%02d%02d", start->tm_year + 1900, start->tm_mon + 1, start->tm_mday, start->tm_hour, start->tm_min, start->tm_sec);
    }
    else
    {
        time_t end_time = event.duration_end;
        struct tm *end = localtime(&end_time);
        sprintf(final, "DTEND;TZID=Europe/Copenhagen:%02d%02d%02dT%02d%02d%02d", end->tm_year + 1900, end->tm_mon + 1, end->tm_mday, end->tm_hour, end->tm_min, end->tm_sec);
    }

    return final;
}

int get_uid()
{

    int r = rand() % 1000000000;
    printf("%d\n", r);
    return r;
}
