#!/bin/bash
for filename in `find . | egrep '\.cpp'`;
do
  gcov-7 -e ".*lib/.*" -e ".*tests/.*" -n -o . $filename > /dev/null;
done
