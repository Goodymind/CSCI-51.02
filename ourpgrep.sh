#!/bin/bash

if [ $# -ne 1 ]; 
then
    echo "Error. Give only 1 process name."
    exit 1
fi

# save process name
PROCESS_NAME="$1"

# get current user's name
CURRENT_USER="$USER"