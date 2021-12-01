#ifndef REGEX_HANDLER_H
#define REGEX_HANDLER_H

#include <regex.h>

enum rules { invalid_input = -1,
             open_calendar,
             clear_calendar,
             next_week,
             previous_week,
             clear_day,
             add_event,
             remove_event,
             add_assignment,
             remove_assignment,
             analyze,
             help,
             close };

#define NUM_RULES 12

static const char *expressions[NUM_RULES] = {
    "^open calendar [0-9]{1,} [0-9]{4}",
    "^clear calendar [0-9]{1,} [0-9]{4}",
    "^next week",
    "^previous week",
    "^clear day (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)",
    "^add event '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{2}:[0-9]{2}|[0-9]{1,2}|[0-9]{1,2},[0-9])",
    "^remove event '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)",
    "^add assignment '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{1,2}|[0-9]{1,2},[0-9])",
    "^remove assignment '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)",
    "^analyze",
    "^help",
    "^close"};

void setup_regex(regex_t *regexs);
int checkregex(char *in, regex_t *regexs);

#endif