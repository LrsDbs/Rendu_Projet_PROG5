#!/bin/sh
i=1
echo "test header section"
for k in *hsection.txt
do
    ./../ARM_runner_example -B ./../Examples_loader/example$i.o > test.txt
    echo "test exemple num $i"
    diff $k test.txt
    i=$(expr $i + 1)
done