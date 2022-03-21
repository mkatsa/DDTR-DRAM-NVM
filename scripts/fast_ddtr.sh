#!/bin/sh

usage="$(basename "$0") <Application> <Num Of Combinations> <Logging> <0(Native)-1(Quartz)-2(Optane)>"

if [ "$#" -ne 4 ]; then
  echo $usage >&2
  exit 1
fi

while test $# -gt 0; do
	case "$1" in
		-h|--help)					#-h flag for help
			echo $usage >&2
			exit 1
			;;
		*)
			executable=$1
			combnumber=$2
			logging=$3
			target=$4			#0 for Native, 1 for Quartz, 2 for Optane
			break
			;;
	esac
done

cd ${FRAMEWORK_ROOT}/ddtrlibrary

if [ -f "config.h" ];
then
	rm -f config.h
fi

echo 'Executing Fast DDTR'
FILE=config.h

DDTs="VECTOR SLLLIST DLLLIST SLLROVINGLIST DLLROVINGLIST"

for type in $DDTs; do
	echo $type
	echo "#ifndef CONFIG_H" >> $FILE
	echo "#define CONFIG_H" >> $FILE

	echo "#include \"definitions.h\"" >> $FILE
	echo "class compositeObject;" >> $FILE

	if [ $logging -eq 1 ] #1 for logging
	then
		if [ -f "log_${type}.out" ];
		then
			rm -f log_${type}.out
		fi
		echo "#define LOGFILENAME \"log_${type}.out\"" >> $FILE
		wait
	else
		if [ -f "log_nolog.out" ];
		then
			rm -f log_nolog.out
		fi
		echo "#define LOGFILENAME \"log_nolog.out\"" >> $FILE
		wait
	fi

	for id in 0 1 2 3; do
		if [ $logging -eq 1 ] #1 for logging
		then
			#echo "#define LOGFILENAME \"log_${combnumber}.out\"" >> $FILE
			echo "typedef ddt_helper<compositeObject, ${id}>::${type} myddtType${id};" >> $FILE
			wait
		else
			#echo "#define LOGFILENAME \"log_nolog.out\"" >> $FILE
			echo "typedef ddt_helper<compositeObject, ${id}>::${type}_NOLOGGING myddtType${id};" >> $FILE
			wait
		fi
	done
	echo "#endif" >> $FILE
	wait

	cd ${EXECUTABLE_ROOT}
	echo $PWD
	make clean

	wait
	if [ $logging -eq 1 ] #1 for logging
	then
	if [ $target -eq 0 ]
	then
		make log=ENABLE emulator=NOENABLE $executable
	fi
	if [ $target -eq 1 ]
	then
		make log=ENABLE emulator=ENABLE $executable
	fi
	if [ $target -eq 2 ]
	then
		make log=ENABLE optane=ENABLE $executable
	fi
	fi

	if [ $logging -eq 0 ] #0 for no logging
	then
	if [ $target -eq 0 ]
	then
		make log=NOENABLE emulator=NOENABLE $executable
	fi
	if [ $target -eq 1 ]
	then
		make log=NOENABLE emulator=ENABLE $executable
	fi
	if [ $target -eq 2 ]
	then
		make log=NOENABLE optane=ENABLE $executable
	fi

	fi
	wait


	./${executable}
	wait

	cd ${FRAMEWORK_ROOT}/ddtrlibrary

	if [ -f "config.h" ];
	then
		rm -f config.h
	fi

done

cd ${FRAMEWORK_ROOT}/scripts