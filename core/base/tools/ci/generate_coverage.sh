#!/bin/bash
#Example: software/airliner/public/core/base/tools/ci/generate_coverage.sh software/airliner/public/build/fixedwing/gemini2/cpd/sitl/target
DIR="$1"
lcov -c -i -b $DIR -d $DIR -o Coverage.baseline
lcov -c -d $DIR -b $DIR -o Coverage.out
lcov -a Coverage.baseline -a Coverage.out -o Coverage.txt
lcov -r Coverage.txt /usr/include/\* -o Coverage.txt
genhtml Coverage.txt -o HTML
