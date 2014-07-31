libfast-mktime
==============

This is a micro-optimization for mktime(3) that will speed it up
around 10x to 86x in the best case and have a slight impact in the worse.

Motivation
----------

The system mktime(3) handles time zones, tzset(), leap years, input
normalization, plus the obvious date calculations, all of which can
take some cycles.  If your use case entails many mktime() calls within
the same calendar day, there is an easy optimization which can show
substantial savings.

A common use case would be parsing a large log file with many timestamps in the same
day and you would like to store the timestamps in epoch time.

Caveats
-------
1. This hack is not thread safe.
2. It does not check if the caller has switched timezones using tzset.

Algorithm
---------

The first call, call system mktime(), memoizing the date and the
beginning of that date in epoch time. Then for subsequent calls, if
they are on the same date, add the requested H:M:S to that saved
midnight, skipping all the date calculations.  If the date is
different, use the system mktime() again and memoize it.

This package provides a libfast-mktime.so.1, defining only mktime()
and intended to be interpose system mktime() as above, using
LD_PRELOAD.

Platforms Tested
----------------

Please let me know if something else works.

<blockquote>
<table width="40%">
  <tr align="left"><th> Platform  <th> GCC   <th> Benchmark </tr>
  <tr><td> i686 GNU/Linux         <td> 4.6.3 <td> ~86x </tr>
  <tr><td> x86_64 GNU/Linux       <td> 4.6.3 <td> ~86x </tr>
  <tr><td> sun4v Solaris sparc VM <td> 4.0.2 <td> ~10x </tr>
</table>
</blockquote>

Building
--------

    $ make
    $ make test
    $ make bench                                

Usage
-----

    $ LD_PRELOAD=./libfast-mktime.so.1 /path/to/your/program

Benchmark
---------

    ## Benchmarking once with stock mktime and once with wrapper.
    ## 3 trials of 1e6 sequential dates will be run.
  
    ./benchmark 3 1000000
      0: 0.868179 s
      1: 0.865169 s
      2: 0.866598 s
    avg: 0.866649 s
    
    LD_PRELOAD=./libfast-mktime.so.1 ./benchmark 3 1000000
      0: 0.014434 s
      1: 0.014622 s
      2: 0.015013 s
    avg: 0.014690 s

License
-------

Copyright (C) 2014 Mitchell Perilstein. Licensed under GNU LGPL
Version 3. See LICENSING file for details.

