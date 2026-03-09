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

pids=$(./ourpgrep.sh "$pattern")
for pid in $pids
do
    kill $signal $pid >/dev/null
done
