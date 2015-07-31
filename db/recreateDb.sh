#!/bin/sh

echo "Recreating database schema..."
sudo -u concordia psql concordia_server -f concordia_server.sql

echo "Inserting initial data..."
for initFile in `ls init/*`
do
    echo "Init file:" $initFile
    sudo -u concordia psql concordia_server -f $initFile
done

echo "Concordia server database recreation complete!"
