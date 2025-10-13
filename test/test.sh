#!/usr/bin/env bash

rm *.out

cat 1.in | ../main > 1.out
cmp 1.out 1.expect

echo "finished test"
