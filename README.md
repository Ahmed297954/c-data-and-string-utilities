# C Data & String Utilities

Modular C programs for reading and converting CSV and binary weather files, searching and sorting arrays with `void *` pointers, and processing text from the command line.

## Folders

* `generic-algorithms/`: Functions to count, search, and sort arrays of any type (`array.h`, `array.c`).
* `meteor-data-pipeline/`: Command-line tools to parse CSV weather files, convert them to binary `.bin` files, and compute temperature or sunshine statistics.
* `text-and-cli-tools/`: Custom string search functions (`str.h`, `str.c`), a command-line character filter (`tintin.c`), and file character masking utilities.

## Compilation

```bash
gcc -std=c2x -Wall -Wextra -O2 meteor-data-pipeline/meteoyearlybin_print.c generic-algorithms/array.c -Igeneric-algorithms -o meteoyearlybin_print
gcc -std=c2x -Wall -Wextra -O2 text-and-cli-tools/main.c text-and-cli-tools/str.c -Itext-and-cli-tools -o str_tests
gcc -std=c2x -Wall -Wextra -O2 text-and-cli-tools/tintin.c -o tintin
```
