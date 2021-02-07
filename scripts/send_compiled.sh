#!/bin/bash
#script to copy compiled

#var
hosts_file=/home/practica/tools/mpi/share/copyon
origin=/home/practica/eclipse-workspace/c/Debug/c
dest=/home/practica/eclipse-workspace/c/Debug/
#

echo "copying"
while IFS= read -r host;do
	echo "$host"
 	scp $origin "$host:$dest"
done <$hosts_file
exit 0