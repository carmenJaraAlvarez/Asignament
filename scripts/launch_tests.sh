#!/bin/bash
#script to launch set of tests

#var
origin=/home/practica/eclipse-workspace/c/Debug/c
slave=192.168.2.115
#

echo "testing"

	mpirun -np 3 -hosts master,$slave $origin test 3 1 1 1 0 0 &
	wait $!
	echo "finished np3 3 1 1 1 0 0"
	mpirun -np 3 -hosts master,$slave $origin test 3 1 1 1 0 1 &
	wait $!
	echo "finished np3 3 1 1 1 0 1"
	mpirun -np 3 -hosts master,$slave $origin test 3 1 1 1 0 2 &
	wait $!
	echo "finished np3 3 1 1 1 0 2"
	mpirun -np 3 -hosts master,$slave $origin test 4 1 1 1 0 0 &
	wait $!
	echo "finished np3 4 1 1 1 0 0"

exit 0
