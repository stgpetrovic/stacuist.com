#!/bin/bash
until WT_APP_ROOT=/root/stacuist.com/resources /usr/bin/spawn-fcgi -n -p 9091 -- /root/stacuist.com/main --db_path /root/stacuist.com/recipes.db; do
  echo "Server crashed, respawning..."
  sleep 1
  killall main
done
