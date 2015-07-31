#!/bin/sh

pidFile="/home/rafalj/projects/concordia-server/scripts/concordia-server.PID"

if [ -e $pidFile ]
then
    pid=`cat $pidFile`
    rm $pidFile
    kill $pid
    echo "concordia-server stopped"
else
    echo "no PID file found at:" $pidFile "- is concordia-server running?" >&2
fi

