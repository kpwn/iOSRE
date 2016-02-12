#!/bin/bash

DIR=/lib/modules/`uname -r`

for i in `find $DIR/kernel -type f -name "*.ko" | grep -v test_nx | grep -v ring_buffer_benchmark`
do
        MOD=`basename $i | sed s/\.ko//`
        echo Loading $MOD
        /sbin/modprobe $MOD
done
