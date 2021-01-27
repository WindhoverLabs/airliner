#!/bin/bash

shopt -s globstar
for file in $1/*.html; do
  echo Converting $(basename $file)
  pandoc --from=html --to=rst $file > ${file%.html}.rst
done
#pandoc --from=html --to=rst %1 
