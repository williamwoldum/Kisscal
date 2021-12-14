#include "../headers/regex_handler.h"

#include <regex.h>
#include <stdio.h>

/************************************************************************* Global variables  */

regex_t regexs[num_input_rules];

/************************************************************************* Global functions  */

/**
 * @brief  Precompiles defined regexs
 * @note
 * @retval None
 */
void setup_regex(void) {
    const char *expressions[num_input_rules] = {
        "^open week: [0-9]{1,2} [0-9]{4}\n",
        "^clear week\n",
        "^next week\n",
        "^previous week\n",
        "^clear: (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
        "^add event: '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} [0-9]{2}:[0-9]{2}\n",
        "^remove event: (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2}\n",
        "^add assignment: '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{1,2}|[0-9]{1,2}.[0-9]) ([0-9]{1,2}|[0-9]{1,2}.[0-9])\n",
        "^remove assignment: (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2}\n",
        "^print: (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
        "^import ics\n",
        "^export ics\n",
        "^analyze\n",
        "^help\n",
        "^close\n"};

    int i;
    for (i = 0; i < num_input_rules; i++) {
        if (regcomp(&regexs[i], expressions[i], REG_ICASE + REG_EXTENDED)) {
            fprintf(stderr, "Could not compile regex\n");
        }
    }
}

/**
 * @brief   Validates if the input matches any of the definded rules, and returns the index of the matched rule
 * @note
 * @param  *in: User input
 * @retval int (index of matched_rule)
 */
int checkregex(char *in) {
    int matched, matched_rule;

    int match = 1, index = 0;
    while (match && index < num_input_rules) {
        matched = !regexec(&regexs[index], in, 0, NULL, 0);

        if (matched) {
            matched_rule = index;
            match = 0;
        } else {
            matched_rule = -1;
        }
        index++;
    }

    return matched_rule;
}