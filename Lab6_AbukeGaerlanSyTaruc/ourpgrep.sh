#!/bin/bash
input=$1
current_user=$USER
char_length=${#current_user}

if [ -n "$input" ]; then
	ps aux | grep "$current_user" | grep "$input" | grep -v grep | cut -c "$((char_length+10))"-"$((char_length+14))"
else
	echo "Please input a program name to search"
fi