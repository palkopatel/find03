#!/bin/sh

if [ "$#" -eq 0 ]; then
    echo
    echo
    echo "Illegal number of parameters. You should put search patterns as args"
    echo
    exit 1
fi

cd db
../find03/find03 --file "$@"
cd ..
