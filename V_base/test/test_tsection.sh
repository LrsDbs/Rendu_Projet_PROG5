#!/bin/sh
i=1
echo "test table section"
for k in *tsection.txt
do
    ./../ARM_runner_example -C ./../Examples_loader/example$i.o > test.txt
    echo "test exemple num $i"
    diff $k test.txt
    i=$(expr $i + 1)
done