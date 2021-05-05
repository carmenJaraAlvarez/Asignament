#!/bin/bash
#script new slave enviroment lab

#var
compiled_dir=/home/practica/eclipse-workspace/c/Debug
#dir slave_env copied by usb for ssh
origin=/home/practica/slave_env/.ssh
dest=/home/practica/

#no needed in lab
#sudo apt-get update
#sudo apt-get install -y mpi
#sudo apt-get install -y mpich ?????

#commands
echo "setting slave env"
mkdir -p $compiled_dir
cp -r $origin $dest
chmod -R 740 $dest
exit 0








