# Paging

## Modified Functions

| Function            | From File    | Usage                                                                   |
| ------------------- | ------------ | ----------------------------------------------------------------------- |
| vm_pageout_scan     | vm_pageout.c | add satistic counters; modify the activity count                        |
| vm_phys_free_pages  | vm_phys.c    | change the add free page algorithm; head/tail determined by even or odd |
| vm_page_aflag_set   | vm_page.h    | add a chance to not set ref bit                                         |
| vm_page_aflag_clear | vm_page.h    | add a chance to not set ref bit                                         |
| _vm_page_deactivate | vm_page.c    | move in-page to front of deactive queue instead of rear                 |
| vm_page_activate    | vm_page.c    | move in-page to front of active queue instead of rear                   |


## Output Data

The program use system log with level DEBUG to record the status when I run the stress program.

The folder `raw` contains the raw debug log files.
The file with `mod` means it's the output of modified algorithm, and `org` represents the original version kernel.
The file with `clear` means the status are cleared after each `vm_pageout_scan`, whereas the file without it means the status will incrementally grow.

The folder `csvs` contains the csv files which are converted from `raw`. 

The folder `compare` contains the csv files reveals the comparsion between **modified algorithm** and **original algorithm** for every status transitions.

The folder `graph` contains the line graphs that are plotted by the files in `compares` folder.

The python script file `dump.py` dump the log to csv.

The csv format can be directly open by darwin's application, Number, which can plot graph easily. Maybe it's also can be imported to Excel, but I didn't try it.


