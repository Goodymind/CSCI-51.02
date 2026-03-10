#!/bin/bash
#source "ourpgrep.sh"

# input="$@"

option=$1
command=$2

if [ -z "$option" ]; then
    echo "Error: Please Provide a pattern" >&2
    exit 1
fi

if [[ "$option" == -[0-9]* ]]; then
    signal=$option
    pattern=$command
else
    # default signal -15
    signal=-15
    pattern=$option
fi

if [ -z "$pattern" ]
then
    echo "Error: Please provide a pattern" >&2
    exit 1
fi

pids=$(./ourpgrep.sh "$pattern")

if [ -z "$pids" ]; then
    echo "Error: no process found matching '$pattern'" >&2
    exit 1
fi

for pid in $pids
do
    kill $signal $pid >/dev/null
done
