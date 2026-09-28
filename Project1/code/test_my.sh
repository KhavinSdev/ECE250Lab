#!/bin/bash

# Path to your compiled C++ program
PROGRAM="./main.exe"

# Make sure the program is executable
if [[ ! -x "$PROGRAM" ]]; then
    echo "Error: $PROGRAM is not executable"
    exit 1
fi

# Run the program and send commands
"$PROGRAM" << EOF

ADD_P1 2000 100
ADD_P1 2005 200
ADD_P1 1995 150
ADD_P1 2002 50
ADD_P1 2000 300
ADD_P1 2002 -1
ADD_P1 1950 10
ADD_P1 2030 20
ADD_P1 2010 -50
ADD_P1 2005 250
ADD_P1 1950 30
ADD_P1 2030 40
ADD_P1 2030 -10
ADD_P1 1998 -5

PRINT_P1

EOF
