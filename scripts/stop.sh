#!/bin/sh

set -e

pidFile="/home/rafalj/projects/concordia-server/scripts/concordia-server.PID"

if [ -e $pidFile ]
then
    kill `cat $pidFile`
    rm $pidFile
    echo "concordia-server stopped"
else
    echo "no PID file found at:" $pidFile "- is concordia-server running?" >&2
    exit 1
fi

