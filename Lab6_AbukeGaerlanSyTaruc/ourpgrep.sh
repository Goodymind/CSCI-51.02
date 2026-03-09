#!/bin/bash

input=$1
current_user=$USER
char_length=${#current_user}

if [ -n "$input" ]; then
	ps aux | grep "$current_user" | grep -w "$input" | grep -v grep | grep -v kill | awk '{print $2}'
else
	echo "Please input a program name to search"
fi
