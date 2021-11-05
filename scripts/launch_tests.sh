#!/bin/bash
#script to launch set of tests

#var
origin=/home/practica/eclipse-workspace/c/Debug/c
master=192.168.2.100
slaves=192.168.2.101,192.168.2.102,192.168.2.103,192.168.2.104
#

echo "testing"

for NP in 1 2 3 4
do
	for PRUNE in 0 1
	do
		for SEARCH in 0 1
		do
			for TUPLE in 0 1 
			do
				for RR in 0 1
				do
					for RRALL in 0 1
					do
						for ALG in 1 2 3
						do
							for SIZE in 3 4 5
							do
								mpirun -np $NP -hosts $master,$slaves $origin test $SIZE $PRUNE $RR $TUPLE $SEARCH $ALG $RRALL &
								wait $!
								echo "End np $NP data $SIZE prune $PRUNE distrib $RR tuple $TUPLE bd $SEARCH alg $ALG rr_all $RRALL"
							done
						done
					done
				done
			done
		done
	done
done
exit 0
