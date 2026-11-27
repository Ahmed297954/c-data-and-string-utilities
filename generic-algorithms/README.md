# Generic Array Utilities

Functions that work on arrays of any type using `void *` pointers, element size (`size_t`), byte-level stepping (`unsigned char *`), and function pointers.

## Files

* `array.h`: Declares `cond_count`, `cond_left_search`, `min_left_search`, and `select_sort`.
* `array.c`: Implements element counting, linear and minimum search, byte-by-byte swapping (`mem_swap`), and selection sort.
