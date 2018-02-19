#!/bin/bash

lcov --capture --initial --directory . --output-file coverage_base.info
lcov --capture --directory . --output-file coverage_test.info
lcov --add-tracefile coverage_base.info --add-tracefile coverage_test.info --output-file coverage_merge.info
lcov --remove coverage_merge.info "*/external/*" "*/tests/*" "*/usr/*" -o coverage.info
lcov --capture --initial --directory . --output-file coverage_base.info
