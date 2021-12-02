#include "../headers/regex_handler.h"

#include <regex.h>
#include <stdio.h>

void setup_regex(regex_t *regexs) {
    int i;
    for (i = 0; i < NUM_RULES; i++) {
        if (regcomp(&regexs[i], expressions[i], REG_ICASE + REG_EXTENDED)) {
            fprintf(stderr, "Could not compile regex\n");
        }
    }
}

int checkregex(char *in, regex_t *regexs) {
    int matched;
    int matched_rule = 0;

    int matching = 1, index = 0;
    while (matching) {
        matched = !regexec(&regexs[index], in, 0, NULL, 0);

        if (matched) {
            matched_rule = index;
            matching = 0;
        } else {
            matched_rule = -1;
        }
        if (++index > NUM_RULES) {
            matching = 0;
        }
    }
    return matched_rule;
}