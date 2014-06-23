# 
# mktime(3) wrapper library
#
# Usage:  make [test|bench|clean]
#
# Copyright (C) 2014 Mitchell Perilstein
# Licensed under GNU LGPL Version 3. See LICENSING file for details.
#

LIB   = libfast-mktime.so.1
ALL   = $(LIB) benchmark unittest

all: $(ALL)

$(LIB): fast-mktime.c
	gcc -Wall -pedantic -fPIC -shared -o $@ $< -ldl

test: $(ALL)
	@echo
	@echo '## Testing once with stock mktime and once with wrapper.'
	@echo '## Expect all "okay" results'
	@echo
	./unittest 
	@echo
	LD_PRELOAD=./$(LIB) ./unittest

bench:
	@echo
	@echo '## Benchmarking once with stock mktime and once with wrapper.'
	@echo '## 3 trials of 1e6 sequential dates will be run.'
	@echo
	./benchmark 3 1000000
	@echo
	LD_PRELOAD=./$(LIB) ./benchmark 3 1000000

clean:
	@rm -f $(ALL)
