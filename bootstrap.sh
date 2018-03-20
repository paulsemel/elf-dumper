#!/bin/sh

cd include

for var in $(cat ../xlat.in); do
  ftmp="$(echo $var | cut -d '.' -f 1).h"
  ls "$ftmp" &> /dev/null
  [ $? -gt 0 ] && ./gen.sh $var $ftmp
done
true
