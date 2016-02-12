#!/bin/bash

for j in `lsmod |awk '{ print $1}' | grep -v caif | grep -v Module`
do
  modprobe -r $j
done

