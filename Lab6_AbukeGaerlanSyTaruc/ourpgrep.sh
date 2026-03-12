#!/bin/bash

input=$1
current_user=$USER

# uses ps aux, filters by usign current user env variable, name of program inputted, and uses awk to get the 2nd variable outputted (PID)
if [ -n "$input" ]; then
	ps aux | grep "$current_user" | grep -w "$input" | grep -v grep | grep -v kill | awk '{print $2}'
else
	echo "Please input a program name to search" >&2
	exit 1 # silently exits when done
fi
