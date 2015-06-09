#!/bin/sh

set -e

nginxProcesses=`ps aux | grep nginx | wc -l`
if [ $nginxProcesses -le 1 ]
then
    echo "nginx is not running" >&2
    exit 1
fi

pidFile="/home/rafalj/projects/concordia-server/scripts/concordia-server.PID"

if [ -e $pidFile ]
then
    echo "concordia server is already running with PID:" `cat $pidFile` >&2
    exit 1
else
    spawn-fcgi -p 8000 -n "/home/rafalj/projects/concordia-server/build/concordia-server"/concordia_server_process &
    echo $! > $pidFile
    echo "concordia-server started with PID:" `cat $pidFile`
fi

