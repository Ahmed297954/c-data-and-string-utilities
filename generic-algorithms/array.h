/*
 * Author: Ahmed BOUAZZA
 * Description: Generic array algorithms header declaring counting, linear/minimum search, and generic selection sort routines.
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stddef.h>

extern size_t cond_count(const void *base, size_t nmemb, size_t size,
    bool (*cond)(const void *));

extern void *cond_left_search(const void *base, size_t nmemb, size_t size,
    bool (*cond)(const void *));

extern void *min_left_search(const void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *));

extern void select_sort(void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *));

#endif
