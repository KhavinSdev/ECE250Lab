#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

for in_file in tests/*.in; do
    base=$(basename "$in_file" .in)
    out_file="tests/$base.out"

    echo "Running test: $base"

    ./main.exe < "$in_file" > tests/actual_$base.results

    if diff --color=always -w tests/actual_$base.results "$out_file"; then
        echo -e "  ${GREEN}PASS${NC}"
    else
        echo -e "  ${RED}FAIL${NC}"
    fi

    echo
done
