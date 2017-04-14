#!/bin/sh

TRINITY_PATH=${TRINITY_PATH:-.}

for core in $(find . -name "core.*")
do
  gdb -batch -n -ex 'bt' $TRINITY_PATH/trinity $core > core.txt
  SHA=$(cat core.txt|  grep -v New\ LWP | sha1sum)
  cat core.txt > core-$SHA.txt
  rm -f core.txt
done

