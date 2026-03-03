#!/bin/bash
input=""
read input
current_user=$USER
char_length=${#current_user}

if [ -n "$input" ]; then
	ps aux | grep "$current_user" | grep "$input" | grep -v grep | cut -c "$((char_length+10))"-"$((char_length+13))"
else
	echo "Please input a program name to search"
fi
