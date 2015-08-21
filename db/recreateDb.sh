#!/bin/sh

echo "Recreating database schema..."
psql -U concordia concordia_server -f concordia_server.sql

echo "Inserting initial data..."
for initFile in `ls init/*`
do
    echo "Init file:" $initFile
    psql -U concordia concordia_server -f $initFile
done

echo "Concordia server database recreation complete!"
