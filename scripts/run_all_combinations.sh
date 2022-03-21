#!/bin/sh

usage="$(basename "$0") <Application> <LoggingOptions>"

if [ "$#" -ne 2 ]; then
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
			logging=$2
			echo "Running all Pareto Optimal Solutions of DDTR"
		
			break
			;;
	esac
done


paretoinput="${SOURCE_CODE_ROOT}/pareto_combinations.txt"

while IFS= read -r comb;     
do
	#echo $executable
	#echo $comb
	
	if [ $executable = "SMS-EMOA-exe" ]; then
		echo "Executing SMS-EMOA"
		./run_shark_combination.sh ${executable} ${comb} ${logging} 0
		echo $PWD
		cd ../Shark-master-ddtr/Test/Algorithms/DirectSearch
		./${executable}
		cd ${FRAMEWORK_ROOT}/ddtrlibrary
	fi
	if [ $executable = "motivation" ]; then
		echo "Executing Motivational Example"
		echo $executable
		./run_combination.sh ${executable} ${comb} ${logging} 0
		cd ${EXECUTABLE_ROOT}
		./${executable}
		cd ${FRAMEWORK_ROOT}/scripts
	fi
done < "$paretoinput"

