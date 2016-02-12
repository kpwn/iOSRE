#!/bin/sh

TRINITY_PATH=${TRINITY_PATH:-.}

# remove old cores
for i in `find . -name "core.*" -exec ls -l {} \; | grep -v "$(date +%b\ %e)" | awk '{ print $9 }'`; do rm -f $i; done

# Remove corrupted cores
find . -empty -name "core.*" -exec rm -f {} \;
for i in $(file core.* | grep -v $TRINITY_PATH/trinity | awk '{ print $1 }'  | sed 's/://'); do rm -f $i; done
