#!/bin/sh
if [ ! -e bin/server.properties ]
then
    mkdir -p bin
    touch bin/server.properties
    {
        echo "max-players=24"
        echo "waiting-time=5"
        echo "score-default=1000000"
        echo "score-trahir-coop=400000"
        echo "score-double-trahir=-200000"
        echo "score-double-coop=100000"
        echo "max-rounds=5"
    } > bin/server.properties
    echo "Configuration succefully generated !"
else
    echo "Configuration already exists..."
fi