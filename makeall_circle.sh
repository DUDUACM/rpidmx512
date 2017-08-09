#!/bin/bash

cd Circle
make -f Makefile $1 $2 || exit

cd ..

DIR=rpi_circle_*
for f in $DIR
do
	echo "[$f]"
	if [ -d $f ]; then
		if [[ $f != *"lib"* ]]; then
			cd "$f"
			if [ -f Makefile ]
				then
					make $1 $2 || exit
			fi
			cd ..
		fi
	fi
done