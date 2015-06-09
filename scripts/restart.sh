#!/bin/sh

set -e

echo "restarting concordia-server"

/home/rafalj/projects/concordia-server/scripts/stop.sh
/home/rafalj/projects/concordia-server/scripts/start.sh
