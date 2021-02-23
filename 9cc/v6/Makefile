
# Makefile Basics
# https://www.gnu.org/software/make/manual/make.html
# http://objectclub.jp/community/memorial/homepage3.nifty.com/masarl/article/gnu-make/rule.html

# compile option
CFLAGS=-std=c11 -g -static

9cc: 9cc.c

test: 9cc
	./test.sh

clean:
	# 9cc: executable file
	# .o: clang object file
	# ~: clang backup file
	# tmp*: executable file or assembly source file
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
