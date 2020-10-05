#ifndef UTIL_ARRAY_H_
#define UTIL_ARRAY_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Returns an array of strings
 * @param arr_ptr pointer to array place
 * @param str string to parse
 * @param delimiter char to split string
 * @code:
 * char **arr, *str = "sen massa  strings";
 * int size, i;
 *
 * size = explode(&arr, str, ' ');
 *
 * for (i = 0; i < size; ++i)
 * printf("\"%s\"\n", arr[i]);
 * printf("\n");
 *
 * free(arr);
 */
int explode(char ***arr_ptr, char *str, char delimiter);

#endif //UTIL_ARRAY_H_
