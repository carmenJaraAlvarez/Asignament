#!/bin/bash
#script to launch jumpshot

#var
clog=/home/practica/c.clog2
slog=/home/practica/c.slog2
#

echo "launch"
clog2TOslog2 "$clog" -o "$slog"
jumpshot "$slog"
exit 0
