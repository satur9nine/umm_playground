#!/bin/bash

# MacOS doesn't support leak detection via ASAN... just skip it

if [ "$(uname)" == "Darwin" ]; then
    export ASAN_OPTIONS="\
    detect_stack_use_after_return=1:\
    check_initialization_order=1:\
    strict_init_order=1:\
    detect_invalid_pointer_pairs=2:\
    strict_string_checks=1:\
    detect_container_overflow=1:\
    report_globals=1:\
    alloc_dealloc_mismatch=1:\
    intercept_tls_get_addr=1:\
    new_delete_type_mismatch=1:\
    print_stats=1:\
    verbosity=1:\
    sleep_before_dying=1"

    ./build/fragtest/fragtest
else
    export ASAN_OPTIONS="\
    detect_leaks=1:\
    detect_stack_use_after_return=1:\
    check_initialization_order=1:\
    strict_init_order=1:\
    detect_invalid_pointer_pairs=2:\
    strict_string_checks=1:\
    detect_container_overflow=1:\
    report_globals=1:\
    alloc_dealloc_mismatch=1:\
    intercept_tls_get_addr=1:\
    new_delete_type_mismatch=1:\
    print_stats=1:\
    verbosity=1:\
    sleep_before_dying=1"

    ./build/fragtest/fragtest
fi
