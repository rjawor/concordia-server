#!/bin/sh

sudo rm -rf index/*
cd db
./recreateDb.sh 
