#!/bin/bash

# Alexander Simchuk, push_updates.sh
# script to push updates made in dev to puppet, and have puppet push dev branch

dev_branch="frecov_updates"

if [ `whoami` != "simca" ]; then
    echo BEGONE UNAUTHORIZED USER!!!
    exit
fi

cd ~/git/puppet-environment
cur_branch=`git branch | grep '*' | cut -d " " -f 2`

echo $cur_branch

if [ "$cur_branch" != "$dev_branch" ] ; then
    printf "Wrong working branch, %s (should be frecov_updates)\n" "$cur_branch"
    exit
fi

# copy over updated files
rsync -PHaz ~/scripts/snapshot-recovery/forms/ modules/internal/cecs/files/profile/frecov_forms/
rsync -PHaz ~/scripts/snapshot-recovery/list_snapshots.sh modules/internal/cecs/files/profile/list_snapshots.sh
rsync -PHaz ~/scripts/snapshot-recovery/frecov modules/internal/cecs/files/profile/frecov

# display status and stage+commit+push if user allows
git status

printf "stage and push branch? (y/n): "
read choice

if [ "$choice" != "y" ]; then
    exit
fi

git add -A :/
git commit -m "made updates"
git push origin frecov_updates
