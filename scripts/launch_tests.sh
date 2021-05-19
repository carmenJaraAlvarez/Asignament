#!/bin/bash
#script to launch set of tests

#var
origin=/home/practica/eclipse-workspace/c/Debug/c
slave=192.168.2.115
#

echo "testing"

for NP in 1 2 5 
do
	for PRUNE in 0 1
	do
		for SEARCH in 0 1
		do
			for TUPLE in 0 1
			do
				for RR in 0 1
				do
					for ALG in 0 1 2
					do
						for SIZE in 3 4 5
						do
							mpirun -np $NP -hosts master,$slave,slave1,slave2 $origin test $SIZE $PRUNE $RR $TUPLE $SEARCH $ALG &
							wait $!
							echo "End np $NP data $SIZE prune $PRUNE distrib $RR tuple $TUPLE bd $SEARCH alg $ALG"
						done
					done
				done
			done
		done
	done
done
exit 0
