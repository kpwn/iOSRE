#!/bin/bash

TRINITY_PATH=${TRINITY_PATH:-.}
TRINITY_TMP=$(mktemp -d /tmp/trinity.XXXXXX)

chmod 755 $TRINITY_TMP
cd $TRINITY_TMP

while [ 1 ];
do
  for syscall in $($TRINITY_PATH/trinity -L | grep entrypoint | grep -v AVOID | awk '{ print $3 }' | sort -u)
  do
	MALLOC_CHECK_=2 $TRINITY_PATH/trinity -q -c $syscall -D &
  done
  wait
done
