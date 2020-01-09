#!/bin/sh
i=1
echo "test symbole"
for k in *symb.txt
do
    ./../ARM_runner_example -D ./../Examples_loader/example$i.o > test.txt
    echo "test exemple num $i"
    diff $k test.txt
    i=$(expr $i + 1)
done