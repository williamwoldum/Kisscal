#ifndef REGEX_HANDLER_H
#define REGEX_HANDLER_H

/************************************************************/

#include <regex.h>

/************************************************************/

enum rules { invalid_input_rule = -1,
             open_week_rule,
             clear_week_rule,
             next_week_rule,
             previous_week_rule,
             clear_day_rule,
             add_event_rule,
             remove_event_rule,
             add_assignment_rule,
             remove_assignment_rule,
             print_day_rule,
             sync_rule,
             export_rule,
             analyze_rule,
             help_rule,
             close_rule,
             num_input_rules };

/************************************************************/

void setup_regex(void);
int checkregex(char *in);

/************************************************************/

#endif