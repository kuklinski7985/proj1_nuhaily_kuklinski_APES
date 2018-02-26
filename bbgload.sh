#!/bin/sh

echo "\n\n********************Running makefile**************************\n\n"

filename=$1
make $filename

echo  "\n\n********************Running scp to copy to BBG****************\n\n"

scp $filename root@192.168.7.2:/root

echo "\n\n********************Entering BBG******************************\n\n"

ssh root@192.168.7.2
