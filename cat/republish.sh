#!/bin/sh

ROOT_PATH="/var/www/html/cat"

sudo rm -rf $ROOT_PATH
sudo mkdir $ROOT_PATH
sudo ./publish.py $ROOT_PATH
