#ifndef REGEX_HANDLER_H
#define REGEX_HANDLER_H

#include <regex.h>

enum rules { invalid_input_rule = -1,
             open_calendar_rule,
             clear_calendar_rule,
             next_week_rule,
             previous_week_rule,
             clear_day_rule,
             add_event_rule,
             remove_event_rule,
             add_assignment_rule,
             remove_assignment_rule,
             analyze_rule,
             help_rule,
             close_rule };

#define NUM_RULES 12

static const char *expressions[NUM_RULES] = {
    "^open calendar [0-9]{1,2} [0-9]{4}\n",
    "^clear calendar [0-9]{1,2} [0-9]{4}\n",
    "^next week\n",
    "^previous week\n",
    "^clear day (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
    "^add event '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{2}:[0-9]{2}|[0-9]{1,2}|[0-9]{1,2},[0-9])\n",
    "^remove event '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
    "^add assignment '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{1,2}|[0-9]{1,2},[0-9])\n",
    "^remove assignment '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
    "^analyze\n",
    "^help\n",
    "^close\n"};

void setup_regex(regex_t *regexs);
int checkregex(char *in, regex_t *regexs);

#endif