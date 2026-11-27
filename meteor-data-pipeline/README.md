# Meteorological Data Tools

Command-line C programs for reading daily weather records (`gyear`, `month`, `mday`, `tx`, `tn`, `rr`, `w`), converting CSV files to binary `.bin` files, and computing statistics or sorting annual records.

## Files

* `meteocsv_avg_w.c`: Reads tab-separated CSV weather files and computes average daily sunshine (`w`).
* `meteocsv_max_tx.c`: Finds the highest maximum temperature (`tx`) in CSV files.
* `meteocsv_ndays_txpp.c`: Counts days with `tx >= 30.0 °C` in CSV files.
* `meteocsv_temp_graph.c`: Prints a terminal text chart showing daily minimum (`-`) and maximum (`+`) temperatures.
* `meteocsv_create_bin.c`: Converts CSV weather files into binary `.bin` files (`fwrite`), storing missing sunshine values as `NAN`.
* `meteobin_max_tx.c`: Reads binary `.bin` weather files (`fread`) and finds the highest `tx`.
* `meteoyearlybin_misc.c`: Loads annual binary files (365–366 days) and reports frost nights (`tn <= 0`), the first day without thaw (`tx <= 0` and `tn <= 0`), and the first lowest `tx`.
* `meteoyearlybin_print.c`: Loads annual binary files and sorts records by `tx` or by `(tx, tn)` using `select_sort`.
