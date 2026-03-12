#!/bin/bash
#source "ourpgrep.sh"

# input="$@"

option=$1
command=$2

# check if no arguments
if [ -z "$option" ]; then
    echo "Error: Please Provide a pattern" >&2
    exit 1
fi

# check if option is a signal
if [[ "$option" = -[0-9]* ]]; then
    signal=$option
    pattern=$command
else #if option is not a signal/ then it must be a pattern
    # default signal -15
    signal=-15
    pattern=$option
fi

# if the set pattern is empty
if [ -z "$pattern" ]
then
    echo "Error: Please provide a pattern" >&2
    exit 1
fi

# external pgrep call
pids=$(./ourpgrep.sh "$pattern")

# no matches
if [ -z "$pids" ]; then
    echo "Error: no process found matching '$pattern'" >&2
    exit 1
fi

# kill each pid
for pid in $pids
do
    # throw result to null
    kill $signal $pid >/dev/null
done
