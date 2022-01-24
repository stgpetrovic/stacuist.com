#!/bin/bash
SLEEP_IN_SECS=3600  #this is 1h
while (( 1 ))
do
        ps -aef | grep main | awk '{print $2}' | while read a
        do
        echo -e "Killing the server..."
        kill ${a}
        done
echo -e "Running the server..."
WT_APP_ROOT=/root/stacuist.com/resources /usr/bin/spawn-fcgi -n -p 9091 -- /root/stacuist.com/main --db_path /root/stacuist.com/recipes.db &
sleep ${SLEEP_IN_SECS}
done
