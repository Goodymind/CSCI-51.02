#!/bin/bash
#source "ourpgrep.sh"

# input="$@"

option=$1
command=$2

if [ -n "$command" ]; then
    signal=$option
    pattern=$command
else
    # default signal -15
    signal=-15
    pattern=$option
fi

for pid in $(./ourpgrep.sh "$pattern")
do
    kill $signal $pid
done
