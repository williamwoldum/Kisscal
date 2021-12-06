#include "../headers/regex_handler.h"

#include <regex.h>
#include <stdio.h>

regex_t regexs[num_input_rules];

void setup_regex() {
    const char *expressions[num_input_rules] = {
        "^open calendar [0-9]{1,2} [0-9]{4}\n",
        "^clear calendar [0-9]{1,2} [0-9]{4}\n",
        "^next week\n",
        "^previous week\n",
        "^clear day (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
        "^add event '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{2}:[0-9]{2}|[0-9]{1,2}|[0-9]{1,2},[0-9])\n",
        "^remove event '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
        "^add assignment '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday) [0-9]{2}:[0-9]{2} ([0-9]{1,2}|[0-9]{1,2}.[0-9])\n",
        "^remove assignment '.{1,}' (Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sunday)\n",
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