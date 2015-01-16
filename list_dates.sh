#!/bin/bash
# Alexander Simchuk, list_dates.sh
# This script finds the bungalow the current users homedir is stored on
# and prints the dates zfs snapshots are available for

bung=`mount | grep /u/$USER | awk -F/ '{print $4}'`

path=/opt/snapshots/bungalow/$bung/.zfs/snapshot

days=`ls $path | grep snap-daily | grep -v latest`

hours=`ls $path | grep snap-hourly-1 | grep -v latest`

echo $path
echo $USER

for h in $hours
do
    echo $h
    echo $h | awk -F- '{print $5"-"$6"-"$4" "$7}'
done

for d in $days
do
    echo $d
    echo $d | awk -F- '{print $5"-"$6"-"$4}'
done
