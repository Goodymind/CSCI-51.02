#!/bin/bash
input=""
read input
current_user=$USER

if [ -n "$input" ]; then
	ps aux | grep "$current_user" | grep "$input" | grep -v grep
else
	echo "Please input a program name to search"
fi
