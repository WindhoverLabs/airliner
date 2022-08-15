#!/bin/bash

DIR="$1"
argc=$#
argv=("$@")

# Add additional exclusions, if specified.
for (( j=1; j<argc; j++ )); do
    lcov -r Coverage.combined ${argv[j]} -o Coverage.combined
    echo lcov -r Coverage.combined ${argv[j]} -o Coverage.combined
done

