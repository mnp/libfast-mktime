libfast-mktime
==============

A C99 mktime(3) library interposer optimized for repeated calls in the same day.

Building
--------

    $ make
    $ make test
    $ make bench				

Usage
-----

    $ gcc your_prog.c
    $ ./a.out			   		  ## regular
    $ LD_PRELOAD=./libfast-mktime.so.1 ./a.out    ## faster?
