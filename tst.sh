#!/bin/sh
# tst.sh - script to run the cprt test program.

OPTS=""
if echo "$OSTYPE" | egrep -i linux >/dev/null; then :
  OPTS="-pthread -l pthread"
fi

gcc -Wall -o gtod_chk $OPTS cprt.c gtod_chk.c
if [ $? -ne 0 ]; then exit 1; fi

./gtod_chk -t 3
