#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

/************************************************************/

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include "./datatypes.h"

/************************************************************/

int prompt_user_input(calendar* current_cal);
void sort_content(day* day);
void load_capped_title(char* str, char* dest, char start_delim, char end_delim);

/************************************************************/

#endif
