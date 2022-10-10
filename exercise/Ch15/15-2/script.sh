#!/usr/bin/env bash

date=$(date +%F);

touch file;
echo -n "# First call to stat at: "; date;
stat file|grep "$date";

sleep 2

echo -n "# Second call to stat at: "; date;
stat file|grep "$date";

rm file;
