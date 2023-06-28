#!/bin/sh

#========================================================================================================
# Usage of script and input args
usage="$(basename "$0") <Application> <Number of DDTs>"

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
			ddts=$2

			if [ -z "${SOURCE_CODE_ROOT}" ]; then
  				echo "Please export SOURCE_CODE_ROOT variable to indicate the absolute path of source code root"
  				exit 1
			else
				SOURCE_CODE_ROOT="${SOURCE_CODE_ROOT}"
				echo "SOURCE_CODE_ROOT=${SOURCE_CODE_ROOT}"
			fi

			if [ -z "${EXECUTABLE_ROOT}" ]; then
  				echo "Please export EXECUTABLE_ROOT variable to indicate the absolute path of executable"
  				exit 1
			else
				EXECUTABLE_ROOT="${EXECUTABLE_ROOT}"
				echo "EXECUTABLE_ROOT=${EXECUTABLE_ROOT}"
			fi

			break
			;;
	esac
done
#========================================================================================================

mkdir -p logs

export FRAMEWORK_ROOT=$PWD

echo ${FRAMEWORK_ROOT}


#========================================================================================================
# Run DDTR and find Pareto Optimal Solutions
cd ./scripts

echo "Executing Design Space Exploration for DDTR"
./fast_ddtr.sh ${executable##*/} $ddts 1 0  #LOGGING #NATIVE


	
python fast_pareto_ddtr.py $ddts ${SOURCE_CODE_ROOT}


./run_all_combinations.sh ${executable##*/} 1 #LOGGING ENABLED
./run_combination.sh ${executable} 0 1 0


cp ${EXECUTABLE_ROOT}/log_* ${FRAMEWORK_ROOT}/logs
cp ${EXECUTABLE_ROOT}/pareto* ${FRAMEWORK_ROOT}/logs
rm ${EXECUTABLE_ROOT}/log_*
rm ${EXECUTABLE_ROOT}/pareto*
#========================================================================================================
