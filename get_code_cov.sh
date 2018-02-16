#!/bin/bash

#lcov --directory . --capture --output-file coverage.info
#lcov --remove coverage.info '/usr/*' "`pwd`/external/*" "`pwd`/tests/*" "`pwd`/lib/*" --output-file coverage.info
#lcov --list coverage.info

#echo "Generating baseline coverage data file."
lcov --capture --initial --directory . --output-file coverage_base.info

#echo "Generating coverage based on executed tests."
lcov --capture --directory . --output-file coverage_test.info

#echo "Merging coverage tracefiles."
lcov --add-tracefile coverage_base.info --add-tracefile coverage_test.info --output-file coverage_merge.info

#echo "Removing unwanted files from coverage."
lcov --remove coverage_merge.info "`pwd`/external/*" "`pwd`/tests/*" "/usr/*" -o coverage.info

#echo "Generating baseline coverage data file."
lcov --capture --initial --directory . --output-file coverage_base.info
