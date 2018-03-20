#!/bin/sh

cd include

for var in $(cat ../xlat.in); do
  rm -f "$(echo $var | cut -d '.' -f 1).h"
done
