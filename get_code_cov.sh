#!/bin/bash

lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' "`pwd`/external/*" "`pwd`/test/*" "`pwd`/lib/*" --output-file coverage.info
lcov --list coverage.info