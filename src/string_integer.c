#include <pelib/string_integer.h>

// Now include the generic set implementation
#define PELIB_PAIR_KEY_T string
#define PELIB_PAIR_VALUE_T int
#include "pelib/pair.c"

#define PELIB_ITERATOR_T pelib_pair_t(string, int)
#include "pelib/iterator.c"

// Now include the generic set implementation
#define MAP_KEY_T string
#define MAP_VALUE_T int
#include "pelib/map.c"

