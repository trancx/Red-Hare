#! /bin/bash

curDir=$(cd "$(dirname "$0")"; pwd)
cd $curDir
if [ -f red-hare ]
then
	cd ..
	if [ -f template.asm ]
	then
		if [ -f a.out ]
		then
			rm a.out	
		fi
		echo -e "\nASSEMBLY CODE"
		echo "------------------------------------------"
		as template.asm && objdump -S < a.out |  egrep  '[ 0-9a-f]+:' | cut -f2 -d":"
		echo "------------------------------------------"
	fi
else
	echo "red-hare does not exist!"	
fi
