#!/bin/bash

tempfile=.temp

for file in "$@"
do
  rm -rf $tempfile
  counter=0
  while read -r line; do
    if [[ $line =~ "#test" ]]; then
      echo -e "#test $file$counter" | sed 's/\.//g' | sed 's/\///g' >> $tempfile
      let "counter+=1"
    else
      echo "$line" >> $tempfile
    fi
  done <$file 
  cat $tempfile > $file
  # cat $tempfile
done

rm -rf $tempfile

