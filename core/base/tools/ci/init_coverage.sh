#!/bin/bash

DIR="$1"

lcov -c -i -b $DIR -d $DIR -o Coverage.baseline
lcov -c -d $DIR -b $DIR -o Coverage.out
lcov -a Coverage.baseline -a Coverage.out -o Coverage.combined
