# Text & Command-Line Tools

Custom C string search functions, command-line filtering, and file character masking utilities.

## Files

* `str.h` / `str.c` / `main.c`: Pointer-based versions of standard string search functions (`str_chr`, `str_rchr`, `str_pbrk`, `str_spn`) and a test program comparing outputs against `<string.h>`.
* `tintin.c`: Filters Tintin character records from command-line arguments by publication year (`strtol`) or name substring (`strstr`), using a `struct` with an `enum` and a `union`.
* `cpbleep_alphaordigit.c`: Copies a text file to a new file (`fopen` with `"wx"` mode) while replacing letters (`isalpha`) or digits (`isdigit`) with a chosen character.
* `chbleep_alphaordigit.c`: Replaces letters or digits in place inside an existing text file (`"r+"` mode with `fseek`).
