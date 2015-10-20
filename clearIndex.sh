#!/bin/sh

rm -rf index/*
cd db
./recreateDb.sh
