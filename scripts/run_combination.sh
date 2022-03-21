#!/bin/sh

usage="$(basename "$0") <Application> <Combination Number> <Logging> <0(Native)-1(Quartz)-2(Optane)>"

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

FILE=config.h


echo "#ifndef CONFIG_H" >> $FILE
echo "#define CONFIG_H" >> $FILE

echo "#include \"definitions.h\"" >> $FILE
echo "class compositeObject;" >> $FILE

combinput="${SOURCE_CODE_ROOT}/log_combinations.out"

while IFS= read -r combline;     
do
	first=${combline%% *}
	if [ $first -eq $combnumber ]
	then
		#echo $first
		found=$combline
		break
	fi
done < "$combinput"

if [ $logging -eq 1 ] #1 for logging
then
	echo "#define LOGFILENAME \"log_${combnumber}.out\"" >> $FILE
	wait
else
	echo "#define LOGFILENAME \"log_nolog.out\"" >> $FILE
	wait
fi

i=0
id=0
for word in $found
do
	if [ $i -lt 2 ]
	then
		i=$(( $i + 1 ))
		continue
	fi
	if [ $logging -eq 1 ] #1 for logging
	then
		#echo "#define LOGFILENAME \"log_${combnumber}.out\"" >> $FILE
		echo "typedef ddt_helper<compositeObject, ${id}>::${word} myddtType${id};" >> $FILE
		wait
	else
		#echo "#define LOGFILENAME \"log_nolog.out\"" >> $FILE
		echo "typedef ddt_helper<compositeObject, ${id}>::${word}_NOLOGGING myddtType${id};" >> $FILE
		wait
	fi

	id=$(( $id + 1 ))
done

echo "#endif" >> $FILE


#cat $FILE
cd ${EXECUTABLE_ROOT}

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

