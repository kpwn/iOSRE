#!/bin/bash

TRINITY_PATH=${TRINITY_PATH:-.}
TRINITY_TMP=$(mktemp -d /tmp/trinity.XXXXXX)

chmod 755 $TRINITY_TMP
cd $TRINITY_TMP

NR_CPUS=`grep ^processor /proc/cpuinfo | /usr/bin/wc -l`
NR_PROCESSES=$(($NR_CPUS * 2))

while [ 1 ];
do
  RND=$RANDOM
  mkdir tmp.$RND
  cd tmp.$RND
  for i in `seq 1 $NR_PROCESSES`
  do
	$TRINITY_PATH/trinity -q -g vm &
  done
  wait
  cd ..
  rm -rf tmp.$RND
done
