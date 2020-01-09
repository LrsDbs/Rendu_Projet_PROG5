#!/bin/sh
i=1
echo "test header"
for k in *header.txt
do
    ./../ARM_runner_example -A ./../Examples_loader/example$i.o > test.txt
    echo "test exemple num $i"
    diff $k test.txt
    i=$(expr $i + 1)
done